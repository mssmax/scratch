// -----------------------------------------------------------------------
// <copyright file="MonitoredCollection.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Reflection;
using ManagedBuilder.UI.Adapters;

namespace ManagedBuilder.UI.Collections
{
    public class MonitoredCollection<T> : BindingList<T>        
    {
        public MonitoredCollection(): base() { }
        public MonitoredCollection(IList<T> list) : base(list) { }

        public IList<T> GetInternalList()
        {
            return Items;
        }

        public event RemoveItemEvent<T> BeforeRemoveItem = delegate { };

        private void InvokeOnRemoveItem(RemoveItemEventArgs<T> args)
        {
            var handler = BeforeRemoveItem;
            if (handler != null) handler(this, args);
        }

        protected override void RemoveItem(int index)
        {
            InvokeOnRemoveItem(new RemoveItemEventArgs<T>(this[index], index));

            base.RemoveItem(index);
        }

        #region Static stuff

        private static bool _isSortable;
        private static PropertyDescriptor _defaultSortingProperty;

        static MonitoredCollection()
        {
            var tType = typeof (T);

            var attribs = tType.GetCustomAttributes(typeof (SortableAttribute), true);

            _isSortable = false;
            _defaultSortingProperty = null;

            if (attribs == null || attribs.Length <= 0) return;

            var sortInfo = attribs[0] as SortableAttribute;
            if (sortInfo == null) return;
            if (string.IsNullOrEmpty(sortInfo.Property)) return;

            _isSortable = true;
            _defaultSortingProperty = CustomTypeConverter.CreatePropDescriptor(tType, sortInfo.Property);
        }

        #endregion

        #region Sorting
        private ListSortDirection _sortDirection = ListSortDirection.Ascending;
        protected override ListSortDirection SortDirectionCore
        {
            get
            {
                return _sortDirection;
            }
        }

        private PropertyDescriptor _sortProperty;
        protected override PropertyDescriptor SortPropertyCore
        {
            get
            {
                return _sortProperty ?? MonitoredCollection<T>._defaultSortingProperty;
            }
        }

        protected override bool SupportsSortingCore
        {
            get
            {
                return MonitoredCollection<T>._isSortable;
            }
        }

        protected override void ApplySortCore(PropertyDescriptor prop, ListSortDirection direction)
        {
            lock (this)
            {
                _sortDirection = direction;
                _sortProperty = prop;


                var list = Items as List<T>;
                if (list == null)
                    return;

                Comparison<T> d = delegate(T src, T dst)
                                      {
                                          var result = 0;

                                          if (_sortProperty == null) return 0;
                                          
                                          var srcVal = _sortProperty.GetValue(src);
                                          var dstVal = _sortProperty.GetValue(dst);

                                          if (srcVal == null) return -1;
                                          if (dstVal == null) return 1;

                                          result = Comparer.Default.Compare(srcVal, dstVal);

                                          if (_sortDirection == ListSortDirection.Descending)
                                              result *= -1;
                                          
                                          return result;
                                      };

                list.Sort(d);
                
            }
        }

        public void Sort()
        {
            ApplySortCore(_defaultSortingProperty, ListSortDirection.Ascending);
        }

        public void Sort(ListSortDirection direction)
        {
            ApplySortCore(_defaultSortingProperty, direction);
        }

        protected override void RemoveSortCore()
        {
            _sortDirection = ListSortDirection.Ascending;
            _sortProperty = null;
        }

        protected override bool IsSortedCore
        {
            get
            {
                for (var i = 0; i < Items.Count - 1; ++i)
                {
                    var lhs = Items[i];
                    var rhs = Items[i + 1];
                    var property = SortPropertyCore;

                    if (property == null) continue;

                    var lhsValue = Equals(lhs, default(T))
                                       ? null : property.GetValue(lhs);
                    var rhsValue = Equals(rhs, default(T))
                                          ? null : property.GetValue(rhs);
                    int result;
                    if (lhsValue == null)
                        result = -1;
                    else if (rhsValue == null)
                        result = 1;
                    else
                        result = Comparer.Default.Compare(lhsValue, rhsValue);

                    if (SortDirectionCore == ListSortDirection.Descending)
                        result = -result;

                    if (result >= 0)
                        return false;
                }

                return true;
            }
        }
        #endregion

        #region TypeDescriptor
        protected class CustomTypeConverter : TypeConverter
        {
            protected class CustomPropertyDescriptor : SimplePropertyDescriptor
            {
                private readonly PropertyInfo _property;
                public CustomPropertyDescriptor(Type componentType, string name, Type propertyType)
                    : base(componentType, name, propertyType)
                {
                    _property = componentType.GetProperty(name);
                }

                public CustomPropertyDescriptor(Type componentType, string name, Type propertyType, Attribute[] attributes)
                    : base(componentType, name, propertyType, attributes)
                {
                    _property = componentType.GetProperty(name);
                }

                public override object GetValue(object component)
                {
                    if (_property == null)
                        throw new Exception("Invalid property");

                    return _property.GetValue(component, null);
                }

                public override void SetValue(object component, object value)
                {
                    if (_property == null)
                        throw new Exception("Invalid property");

                    _property.SetValue(component, value, null);
                }
            }

            public static PropertyDescriptor CreatePropDescriptor(Type componentType, string name)
            {
                var property = componentType.GetProperty(name);
                return new CustomPropertyDescriptor(componentType, name, property.PropertyType);
            }
        }
        #endregion
    }

    public delegate void RemoveItemEvent<T>(object sender, RemoveItemEventArgs<T> args);

    public class RemoveItemEventArgs<T> : EventArgs
    {
        public T Item { get; private set; }
        public int Index { get; set; }

        public RemoveItemEventArgs(T item, int index)
        {
            Item = item;
            Index = index;
        }
    }

    
}