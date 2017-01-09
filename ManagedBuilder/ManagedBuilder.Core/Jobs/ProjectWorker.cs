// -----------------------------------------------------------------------
// <copyright file="ProjectWorker.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using ManagedBuilder.Core.Exceptions;
using ManagedBuilder.Core.Factories;
using ManagedBuilder.Core.Model;

namespace ManagedBuilder.Core.Jobs
{
    public class ProjectWorker : IWorker<Project>
    {
        private static readonly log4net.ILog log = log4net.LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);

        private Project _project = null;
        private bool _hasExecuted = false;
        private readonly List<IWorker<Project>> _dependencies = new List<IWorker<Project>>();
        private readonly ReaderWriterLock _lock = new ReaderWriterLock();
        private bool _hasFailed;

        public ProjectWorker()
        {
            
        }

        public Project Data
        {
            get { return _project; }
            set { _project = value; }
        }

        public IMaster<Project> Master { get; set; }        

        public void ResolveDependencies()
        {
            ////////////////////////////////////////////////////////////////////
            // contract requirements 

            if (Master == null) 
                throw new InvalidOperationException("Cannot resolve dependencies: Master not set");

            if (_project == null) 
                throw new InvalidOperationException("Cannot resolve dependencies: Project not set");
            
            ////////////////////////////////////////////////////////////////////

            StringBuilder missingDependencies = null;

            _lock.AcquireWriterLock(-1);
            try
            {

                foreach (var dep in _project.Dependencies)
                {
                    var depInstance = Master.GetWorkerByName(dep);
                    if (depInstance == null)
                    {
                        if (missingDependencies == null) missingDependencies = new StringBuilder();
                        if (missingDependencies.Length > 0) missingDependencies.Append(',');
                        missingDependencies.Append(dep);
                        continue;                        
                    }

                    _dependencies.Add(depInstance);
                }

                if (missingDependencies != null)
                    throw new UnresolvedDependencyException(_project.Name, missingDependencies.ToString());
            }
            finally
            {
                _lock.ReleaseWriterLock();
            }
        }

        public bool SatisfyDependency(IWorker<Project> depWorker)
        {
            if (Master == null || _project == null) 
                return false; // TODO: throw exception

            _lock.AcquireWriterLock(-1);
            try
            {
                // find depencendy with same name and remove it
                var idx = _dependencies.IndexOf(depWorker);
                if (idx == -1)
                {
                    //log.DebugFormat("Could not satisfying dependency on [{0}]: [{1}]",
                    //                this.Name,
                    //                depWorker.Name);
                    return false;
                }

                _dependencies.RemoveAt(idx);                
            }
            finally
            {
                _lock.ReleaseWriterLock();
            }

            return true;
        }

        public IWorker<Project>[] UnsatisfiedDependencies()
        {
            lock (_dependencies)
            {
                return _dependencies.ToArray();
            }
        }

        public bool CanExecute()
        {
            _lock.AcquireReaderLock(-1);
            
            try
            {
                return _dependencies.Count == 0;
            }
            finally
            {
                _lock.ReleaseReaderLock();
            }
        }

        public void Execute()
        {
            _lock.AcquireWriterLock(-1);

            try
            {
                if (_dependencies.Count > 0)
                    throw new Exception("Not all dependencies are satisfied");
                
                LastException = null;
                _hasExecuted = false;
                _hasFailed = false;

                BuilderFactory.GetBuilderByFramework(_project.Framework).Execute(_project);                
            }        
            catch(Exception e)
            {
                _hasFailed = true;
                LastException = e;
            }
            finally
            {
                _hasExecuted = true;
                _lock.ReleaseWriterLock();
            }
        }

        public Exception LastException
        {
            get; private set;
        }

        public bool HasFailed()
        {
            return _hasFailed;
        }

        public bool HasExecuted()
        {
            return _hasExecuted;
        }

        public string Name
        {
            get { return _project.Name; }
            set { }
        }
    }
}