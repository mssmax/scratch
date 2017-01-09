// -----------------------------------------------------------------------
// <copyright file="BuilderException.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using ManagedBuilder.Core.Model;

namespace ManagedBuilder.Core.Exceptions
{
    public class BuilderException : Exception
    {
        public BuilderException(string message, Project p) : base(message)
        {
            ExitCode = int.MinValue;
            Project = p;
        }

        public BuilderException(string message, Project p, Exception innerException)
            : base(message, innerException)
        {
            ExitCode = int.MinValue;
            Project = p;
        }

        public Project Project { get; private set; }

        public int ExitCode { get; private set; }

        public static BuilderException CreateFailedToStartException(Project p)
        {
            return new BuilderException("Failed to start builder", p);
        }

        public static BuilderException CreateFailedToStartException(Project p, Exception innerException)
        {
            return new BuilderException("Failed to start builder", p, innerException);
        }

        public static BuilderException CreateBuildFailedDueToExitCodeException(Project p, int exitCode)
        {
            return new BuilderException("Failed to build project", p)
                       {
                           ExitCode = exitCode
                       };
        }

        public override string Message
        {
            get
            {
                var baseMessage = base.Message;

                var projectPart = (Project == null)
                                      ? string.Empty
                                      : string.Format("\r\nProject: {0} [{1}]", Project.Name, Project.Framework);

                var exitCodePart = (ExitCode == int.MinValue) 
                    ? string.Empty
                    : string.Format("\r\nExit Code: {0}", ExitCode);

                return string.Format("{0}{1}{2}", 
                    baseMessage, 
                    projectPart,
                    exitCodePart);

            }
        }
    }
}