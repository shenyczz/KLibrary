using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace WindowsFormsApplication1
{

    [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Auto)]
    public struct Simple
    {
        public short Value;
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Auto)]
    public struct Student
    {
        public short Age;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
        public String Name;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
        public String Address;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
        public double[] Scores;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Auto)]
    public struct Group
    {
        public Int16 GroupID;

        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
        public String Leader;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
        public Student[] Scores;
    }

}
