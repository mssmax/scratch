// -----------------------------------------------------------------------
// <copyright file="ArgumentParser.cs" company="GFI Software Development Ltd.">
//      Copyright (c) GFI Software All rights reserved.
// </copyright>
// -----------------------------------------------------------------------
using System;
using System.Collections.Generic;

namespace ManagedBuilder.CommandLine
{
    public class ArgumentParser
    {
        private Dictionary<string, string> _enabledSwitches = new Dictionary<string, string>();


        public char SwitchChar { get; private set; }

        public List<string> SupportedSwitches
        {
            get; private set;
        }        

        public ArgumentParser(params string[] switches) : this('-', switches)
        {
            
        }
        public ArgumentParser(char switchChar, params string[] switches)
        {
            SwitchChar = switchChar;
            SupportedSwitches = new List<string>(switches);
        }

        public void Parse(string[] args)
        {
            string lastArg = null;
            foreach (var s in args)
            {
                if (s[0] == SwitchChar)
                {
                    var arg = s.Substring(1);
                    if (!IsSupportedSwitch(arg))
                    {
                        throw new InvalidSwitchException(arg);
                    }
                    lastArg = arg;
                    _enabledSwitches.Add(lastArg.ToLower(), "");
                }
                else
                {
                    if (lastArg != null)
                    {
                        _enabledSwitches[lastArg.ToLower()] = s;
                    }
                    else
                    {
                        throw new SwitchNotSpecifiedException();
                    }
                }
            }            
        }

        private bool IsSupportedSwitch(string arg)
        {
            Predicate<string> d = x => x.Equals(arg, StringComparison.CurrentCultureIgnoreCase);
            return SupportedSwitches.Find(d) != null;
        }

        public bool IsSwitchSet(string switchName)
        {
            if (!IsSupportedSwitch(switchName))
            {
                throw new InvalidOperationException(
                            string.Format("Unsupported switch [{0}]", switchName));
            }

            return (_enabledSwitches.ContainsKey(switchName.ToLower()));
        }        

        public string this[string argname]
        {
            get
            {
                return !IsSwitchSet(argname) 
                    ? null 
                    : _enabledSwitches[argname];
            }
        }
    }

    public class ArgumentParserException : Exception
    {        
        public ArgumentParserException(string message) : base(message) { }
        public ArgumentParserException(string fmt, params object[] args) : base(string.Format(fmt, args)) { }
    }

    public class SwitchNotSpecifiedException : ArgumentParserException
    {
        public SwitchNotSpecifiedException() : base("No switch was specified in the argument list") { }
    }

    public class InvalidSwitchException : ArgumentParserException
    {
        public InvalidSwitchException(string switchName) : 
            base("Switch not supported [{0}]", switchName)
        {
            
        }
    }
}