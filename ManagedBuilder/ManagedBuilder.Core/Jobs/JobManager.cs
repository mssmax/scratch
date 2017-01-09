// -----------------------------------------------------------------------
// <copyright file="JobManager.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using ManagedBuilder.Core.Model;
using ManagedBuilder.Core.Performance;

namespace ManagedBuilder.Core.Jobs
{
    public class JobManager : IMaster<Project>
    {
        public string Name { get; private set; }

        public bool ResolveDependencies { get; set; }

        private static readonly log4net.ILog log = log4net.LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);

        private readonly List<IWorker<Project>> _workerList = new List<IWorker<Project>>();
        private readonly List<IWorker<Project>> _executionList = new List<IWorker<Project>>();
        private readonly ReaderWriterLock _primaryLock = new ReaderWriterLock();
        private readonly Object _executionLock = new object();
        private readonly Semaphore _sem = new Semaphore(ManagedBuilderSettings.MaximumBuilders, ManagedBuilderSettings.MaximumBuilders);
        private readonly AutoResetEvent _executionListEmptyEvent = new AutoResetEvent(false);
        private readonly AutoResetEvent _projectFinishedEvent = new AutoResetEvent(false);

        #region Properties

        public int Count
        {
            get { return _workerList.Count; }
        }

        #endregion

        #region c'tor
        public JobManager() : this("Default") { }

        public JobManager(string instanceName)
        {
            Name = instanceName;
            ResolveDependencies = true;
        }
        #endregion

        #region Methods
        public bool ContainsWorkerData(Project workerData)
        {
            return _workerList.Find(x => x.Data == workerData) != null;
        }

        public bool ContainsWorker(IWorker<Project> worker)
        {
            return _workerList.Contains(worker);
        }

        public IWorker<Project> GetWorkerByName(string name)
        {
            _primaryLock.AcquireReaderLock(-1);

            try
            {
                var item = _workerList.Find(x => x.Name == name);

                return item;
            }

            finally
            {
                _primaryLock.ReleaseReaderLock();
            }
        }

        public void AddWorker(string name, Project data)
        {
            if (!Monitor.TryEnter(_executionLock))
                throw new Exception("Cannot modify worker queue while executing");

            IWorker<Project> worker = new ProjectWorker {Data = data, Master = this};

            _primaryLock.AcquireWriterLock(-1);
            try
            {
                _workerList.Add(worker);
            }
            finally
            {
                _primaryLock.ReleaseWriterLock();
            }            
        }

        public void Execute()
        {
            if (!Monitor.TryEnter(_executionLock))
                throw new Exception("Cannot call execute twice");

            var buildIterationIndex = 0;

            if (_workerList.Count == 0)
            {                
                throw new Exception("Worker list is empty ...");
            }

            try
            {
                var perf = JobPerformance.CreateJobStatistics(Name);
                perf.SetTotalJobs(_workerList.Count);

                if (ResolveDependencies)
                {
                    foreach (var worker in _workerList)
                    {
                        worker.ResolveDependencies();
                    }
                }

                var unsatisfiedDeps = new StringBuilder();

                do
                {
                    var workerCount = 0;
                    var bWorkersActive = false;

                    // determine projects that will execute in this iteration                    
                    log.DebugFormat("Beginning build iteration #{0}", ++buildIterationIndex);

                    var localWorkerList = _workerList.ToArray();
                    log.DebugFormat("Remaining items {0}", localWorkerList.Length);

                    foreach (var worker in localWorkerList)
                    {
                        if (!worker.CanExecute())
                        {
                            unsatisfiedDeps.Length = 0;
                            foreach (var dep in worker.UnsatisfiedDependencies())
                                if (unsatisfiedDeps.Length > 0)
                                    unsatisfiedDeps.AppendFormat(",{0}", dep.Name);
                                else
                                    unsatisfiedDeps.AppendFormat("{0}", dep.Name);

                            log.DebugFormat("Skipping {0} [{1}]", 
                                           worker.Name,
                                           unsatisfiedDeps.ToString());
                            
                                
                            continue;
                        }

                        bWorkersActive = true;

                        log.InfoFormat("Building {0}", worker.Name);

                        _workerList.Remove(worker);

                        _sem.WaitOne(-1, false);

                        if (_prematureExit)
                            break;

                        lock (_executionList)
                        {
                            _executionList.Add(worker);
                        }                        
                        
                        _workerExecuteFunc.BeginInvoke(
                            worker,
                            WorkerExecuteInvokationCallback,
                            new WorkerCallbackContext(perf, worker));

                        workerCount++;
                        perf.NewJobExecuted();
                    }      

                    // check if some project failed
                    if (_prematureExit)
                        break;

                    if (!bWorkersActive)
                    {                        
                        log.Debug("No Workers where started, waiting for a project to finish");
                        _projectFinishedEvent.WaitOne(-1, false);
                        log.Debug("Waiting ... returned");
                    }
                    
                        log.DebugFormat("Executing {0} workers", workerCount);
                        log.DebugFormat("Pending {0} workers", _workerList.Count);

                    log.DebugFormat("{0},===========================================", buildIterationIndex);
                } while ((_workerList.Count > 0 || _executionList.Count > 0) && !_prematureExit);

                log.InfoFormat("Building complete ...");
            }
            finally
            {
                Monitor.Exit(_executionLock);
            }
        }
        #endregion

        #region callbacks
        private void WorkerExecuteInvokationCallback(IAsyncResult ar)
        {
            _workerExecuteFunc.EndInvoke(ar);

            var callbackCtx = ar.AsyncState as WorkerCallbackContext;
            if (callbackCtx == null)
            {
                log.Warn("WorkerExecuteCallback was called with an invalid asyncstate");
                return;
            }

            var worker = callbackCtx.Worker;

            if (worker.HasFailed())
            {
                log.Error("Build failed", worker.LastException);
                lock (this)
                {
                    _prematureExit = true;
                }
            }
            else
            {
                log.DebugFormat("{0}, Building complete ...", worker.Name);

                IWorker<Project>[] workersArray = null;
                lock (_workerList)
                {
                    workersArray = _workerList.ToArray();
                }

                var updatedDepList = new StringBuilder();

                log.DebugFormat("{0}, Updating dependencies on {1} waiting workers", worker.Name, workersArray.Length);
                foreach (var remaininWorker in workersArray)
                {
                    if (!remaininWorker.SatisfyDependency(worker)) continue;

                    updatedDepList.Append(remaininWorker.Name);
                    updatedDepList.Append(' ');
                }
                log.DebugFormat("{0}, Satisfying dependency on [{1}]",
                    worker.Name,
                    updatedDepList);
            }

            log.DebugFormat("{0}, Updating performance counters and exec list", worker.Name);
            lock (_executionList)
            {
                callbackCtx.JobPerformance.DecrementActiveItems();
                _executionList.Remove(worker);                
            }

            log.DebugFormat("{0}, Notifying of worker termination", worker.Name);
            _projectFinishedEvent.Set();

            log.DebugFormat("{0}, Releasing semaphore", worker.Name);
            _sem.Release();            
        }

        readonly Action<IWorker<Project>> _workerExecuteFunc = w => w.Execute();
        private bool _prematureExit;

        private class WorkerCallbackContext
        {
            public JobPerformance JobPerformance;
            public IWorker<Project> Worker;

            public WorkerCallbackContext(JobPerformance jobPerformance, IWorker<Project> worker)
            {
                JobPerformance = jobPerformance;
                Worker = worker;
            }
        }
        #endregion
    }
}
