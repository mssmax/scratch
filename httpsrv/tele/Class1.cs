using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using HTTPSrv;

namespace tele
{
    public class TeleTest : IHTTPHandler
    {
        public override bool HandleRequest(string verb, string handlerParam, Dictionary<string, string> parms, HttpListenerContext httpCtx)
        {
            StringBuilder sb = new StringBuilder();
            sb.AppendFormat("C# tele - Verb: {0}\n", verb);
            sb.AppendFormat("Handler param: {0}\n", handlerParam);
            sb.Append("Parameters:\n");
            foreach(string key in parms.Keys)
            {
                sb.AppendFormat("name [{0}], value[{1}]\n", key, parms[key]);
            }
            byte []bytes = Encoding.GetEncoding(1251).GetBytes(sb.ToString());
            httpCtx.Response.ContentType = "text/plain";
            httpCtx.Response.ContentLength64 = bytes.Length;
            httpCtx.Response.Close(bytes, true);
            return true;
        }
    }
}
