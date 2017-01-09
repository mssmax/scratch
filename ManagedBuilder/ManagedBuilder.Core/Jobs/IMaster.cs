// -----------------------------------------------------------------------
// <copyright file="IMaster.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using ManagedBuilder.Core.Model;

namespace ManagedBuilder.Core.Jobs
{
    public interface IMaster<TWorkerData>
    {
        IWorker<Project> GetWorkerByName(string name);
        void Execute();
        int Count { get; }
        bool ContainsWorker(IWorker<Project> worker);
        bool ContainsWorkerData(Project workerData);
    }
}