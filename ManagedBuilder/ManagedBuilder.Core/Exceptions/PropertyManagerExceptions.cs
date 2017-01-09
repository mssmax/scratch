// -----------------------------------------------------------------------
// <copyright file="PropertyManagerExceptions.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;

namespace ManagedBuilder.Core.Exceptions
{
    public class PropertyNotFoundException : Exception
    {
        public PropertyNotFoundException(string propertyName) 
            : base(string.Format("Property [{0}] cannot be found", propertyName))
        {
            
        }
    }

    public class PropertyAlreadyExistsException : Exception
    {
        public PropertyAlreadyExistsException(string propertyName)
            : base(string.Format("Property [{0}] already exists", propertyName))
        {}
    }

    public class PropertyNameMissingException : Exception
    {
        public PropertyNameMissingException(string srcString)
            : base(string.Format("Empty property placement found [{0}]", srcString))
        {

        }
    }
}