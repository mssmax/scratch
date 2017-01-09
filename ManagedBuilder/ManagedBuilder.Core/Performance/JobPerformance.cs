// -----------------------------------------------------------------------
// <copyright file="JobPerformance.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace ManagedBuilder.Core.Performance
{
    public class JobPerformance : IDisposable
    {
        private const string PerfCategoryName = "ManagedBuilder JobManager";
        private const string PerfActiveItemsName = "Active Jobs";
        private const string PerfTotalItemsName = "Number Of Jobs";
        private const string PerfRemainingItemsName = "Remaining Jobs";
        private const string PerfExecutedJobsName = "Executed Jobs";
        
        private readonly PerformanceCounter _activeJobs;
        private readonly PerformanceCounter _executedJobs;
        private readonly PerformanceCounter _remainingJobs;
        private readonly PerformanceCounter _totalJobs;

        public string JobName { get; private set; }

        private JobPerformance(string jobName)
        {
            JobName = jobName;

            _activeJobs = new PerformanceCounter(
                PerfCategoryName,
                PerfActiveItemsName, jobName,
                false) {RawValue = 0};

            _executedJobs = new PerformanceCounter(
                PerfCategoryName,
                PerfExecutedJobsName, jobName,
                false) {RawValue = 0};

            _remainingJobs = new PerformanceCounter(
                PerfCategoryName,
                PerfRemainingItemsName, jobName,
                false) {RawValue = 0};

            _totalJobs = new PerformanceCounter(
                PerfCategoryName,
                PerfTotalItemsName, jobName,
                false) { RawValue = 0 };
        }

        //

        private static readonly Dictionary<string, JobPerformance> Instances =
            new Dictionary<string,JobPerformance>();

        static JobPerformance()
        {
            var counterData = new CounterCreationDataCollection();

            var bCategoryExists = PerformanceCounterCategory.Exists(PerfCategoryName);
            if (!bCategoryExists || !PerformanceCounterCategory.CounterExists(PerfActiveItemsName, PerfCategoryName))
            {
                var counter = new CounterCreationData
                                  {
                                      CounterName = PerfActiveItemsName,
                                      CounterType = PerformanceCounterType.NumberOfItems32
                                  };
                counterData.Add(counter);
            }


            if (!bCategoryExists || !PerformanceCounterCategory.CounterExists(PerfTotalItemsName, PerfCategoryName))
            {
                var counter = new CounterCreationData
                                  {
                                      CounterName = PerfTotalItemsName,
                                      CounterType = PerformanceCounterType.NumberOfItems32                                      
                                  };
                counterData.Add(counter);
            }

            if (!bCategoryExists || !PerformanceCounterCategory.CounterExists(PerfRemainingItemsName, PerfCategoryName))
            {
                var counter = new CounterCreationData
                                  {
                                      CounterName = PerfRemainingItemsName,
                                      CounterType = PerformanceCounterType.NumberOfItems32
                                  };
                counterData.Add(counter);
            }
            
            if (counterData.Count > 0)
            {
                PerformanceCounterCategory.Create(
                    PerfCategoryName,
                    "Performance information for the execution list in the ManagedBuilder",
                    PerformanceCounterCategoryType.MultiInstance,
                    counterData);
            }            
        }

        public static JobPerformance CreateJobStatistics(string jobName)
        {
            var jp = new JobPerformance(jobName);

            Instances.Add(jobName, jp);

            return jp;
        }

        private bool _bDisposing;

        public void Dispose()
        {
            lock (this)
            {
                if (_bDisposing)
                    return;

                _bDisposing = true;
            }

            _activeJobs.RawValue = 0;
            _activeJobs.RemoveInstance();
            _activeJobs.Dispose();

            _executedJobs.RawValue = 0;
            _executedJobs.RemoveInstance();
            _executedJobs.Dispose();

            _remainingJobs.RawValue = 0;
            _remainingJobs.RemoveInstance();
            _remainingJobs.Dispose();
        }

        public void IncrementActiveItems()
        {            
            _activeJobs.Increment();
        }

        public void DecrementActiveItems()
        {
            _activeJobs.Decrement();
        }

        public void SetTotalJobs(int jobs)
        {
            lock (this)
            {
                _remainingJobs.RawValue = jobs;
                _totalJobs.RawValue = jobs;
            }
        }

        public void NewJobExecuted()
        {
            lock(this)
            {
                _activeJobs.Increment();
                _executedJobs.Increment();
                _remainingJobs.Decrement();
            }
        }
    }
}