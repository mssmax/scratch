// -----------------------------------------------------------------------
// <copyright file="SortableAttribute.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;

namespace ManagedBuilder.UI.Collections
{
    public class SortableAttribute : Attribute
    {
        public string Property { get; private set; }

        public SortableAttribute(string property)
        {
            Property = property;
        }
    }
}