// -----------------------------------------------------------------------
// <copyright file="PropertyManager.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Data;
using System.Text;
using ManagedBuilder.Core.Exceptions;

namespace ManagedBuilder.Core.Configuration
{
    public class PropertyManager
    {
        #region Statics
        public static PropertyManager Common { get; private set; }

        static PropertyManager()
        {
            Common = new PropertyManager();

            // add common properties
            Common.Add("system.processorcount", Environment.ProcessorCount.ToString());
        }
        #endregion
        
        private List<Property> _properties = new List<Property>();

        public PropertyManager()
        {
            
        }

        private bool TryGetPropertyByName(string name, bool searchCommon, out Property prop)
        {
            Predicate<Property> d = x => x.Name.Equals(name);
            prop = _properties.Find(d) ?? Common._properties.Find(d);            

            return prop != null;
        }

        private bool TryGetPropertyByName(string name, out Property prop)
        {
            return TryGetPropertyByName(name, true, out prop);
        }

        private Property GetPropertyByName(string name, bool searchCommon)
        {
            Predicate<Property> d = x => x.Name.Equals(name);
            var prop = _properties.Find(d) ?? Common._properties.Find(d);

            if (prop == null)
                throw new PropertyNotFoundException(name);

            return prop;
        }

        private Property GetPropertyByName(string name)
        {
            return GetPropertyByName(name, true);
        }

        public string this[string name]
        {
            get
            {
                lock (this)
                {
                    var prop = GetPropertyByName(name);
                    return prop.Value;
                }
            }
            set
            {
                lock (this)
                {
                    Property prop = null;
                    if (!TryGetPropertyByName(name, out prop))
                        _properties.Add(new Property() {Name = name, Value = value});
                    else
                        prop.Value = value;
                }
            }
        }

        // removes a property from the local store
        public bool Remove(string propertyName)
        {
            var prop = _properties.Find(x => x.Name.Equals(propertyName));
            return prop != null && _properties.Remove(prop);
        }

        public void Add(string name, string value)
        {
            lock (this)
            {
                Property prop = null;
                if (!TryGetPropertyByName(name, out prop))
                    _properties.Add(new Property() {Name = name, Value = value});
                else
                    throw new PropertyAlreadyExistsException(name);
            }
        }

        public void AddDirectoryProperty(string name, string path)
        {
            if (!path.EndsWith("\\"))
            {
                Add(name, path + "\\");
            }
            else
            {
                Add(name, path);
            }
        }

        public string Evaluate(string srcString)
        {
            var target = new StringBuilder();
            var prop = new StringBuilder();

            var state1 = false;         // dollar sign found
            var state2 = false;         // open curly bracket found (i.e. we are 
                                        // processing a prop if state1 && state2 == true)

            
            for (var i = 0; i<srcString.Length; i++)
            {
                var ch = srcString[i];

                switch (ch)
                {
                    case '$':
                        state1 = true;
                        break;
                    case '{':
                        state2 = true;
                        break;
                    case '}':
                        if (state1 && state2)
                        {
                            state1 = state2 = false;

                            // process property
                            if (prop.Length == 0)
                                throw new PropertyNameMissingException(srcString);

                            var propValue = this[prop.ToString()];
                            target.Append(propValue);

                            prop.Length = 0;
                        }
                        else
                        {
                            target.Append(ch);
                        }
                        break;
                    default:
                        if (state1 && state2)
                            prop.Append(ch);
                        else
                        {
                            if (state1)
                                target.Append('$');

                            if (state2)
                                target.Append('{');

                            target.Append(ch);
                        }
                        break;
                }
            }

            return target.ToString();
        }
    }

    

    public class Property
    {
        public string Name { get; set; }

        public string Value { get; set; }

        public override string ToString()
        {
            return string.Format("{0} = {1}", Name, Value);
        }
    }
}