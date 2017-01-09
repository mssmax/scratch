// -----------------------------------------------------------------------
// <copyright file="IEditableObject.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
namespace ManagedBuilder.UI
{
    public interface IEditableObject
    {
        bool IsDirty { get; }        
    }
}