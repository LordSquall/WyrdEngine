using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WyrdGen.Extensions
{
    public static class StringExtension
    {
        public static string ReplaceTag(this string str, string tag, string src)
        {
            return str.Replace("<!" + tag + ">", src);
        }
    }
}
