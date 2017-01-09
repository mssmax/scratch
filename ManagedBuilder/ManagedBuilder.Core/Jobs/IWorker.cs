// -----------------------------------------------------------------------
// <copyright file="IWorker.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using ManagedBuilder.Core.Model;

namespace ManagedBuilder.Core.Jobs
{
    public interface IWorker<TWorkerData>
    {
        /// <summary>
        /// The CanExecute() method is used to check if all the conditions required to execute
        /// a worker are met.  For example: for projects, this would check that all the dependencies
        /// required for that project have executed.
        /// </summary>
        /// <returns>True if its okay to execute, false otherwise</returns>
        bool CanExecute();
        void Execute();
        bool HasExecuted();

        string Name { get; set; }

        TWorkerData Data { get; set; }
        IMaster<TWorkerData> Master { get; set; }
        Exception LastException { get; }
        void ResolveDependencies();
        bool SatisfyDependency(IWorker<Project> depWorker);
        IWorker<Project>[] UnsatisfiedDependencies();
        bool HasFailed();
    }
}