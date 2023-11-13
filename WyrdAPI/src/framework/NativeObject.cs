﻿using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;

namespace WyrdAPI
{
    [StructLayout(LayoutKind.Sequential)]
    public class NativeObject 
    {
        private IntPtr _NativePtr;

        public IntPtr NativePtr
        {
            get
            {
                return _NativePtr;
            }
            set
            {
                _NativePtr = value;
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;
        public void NotifyPropertyChanged(string property)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(property));
            }
        }
    }
}
