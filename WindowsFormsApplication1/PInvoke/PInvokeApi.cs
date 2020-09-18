using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;


namespace WindowsFormsApplication1
{
    class PInvokeApi : TestApi
    {

        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern int GetValue(int iValue);

        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern IntPtr GetString(String str);
        //-------------------------------------------------
       


        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern void GetArray(int[] ary, int length);

        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern void GetArrayPtr(int[] ary, int length);

        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern void GetArrayRef(int[] ary, int length);
        //-------------------------------------------------



        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern void GetArrayDoubleIntPtr(out IntPtr p, out int length);

        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern IntPtr GetArrayDoubleIntPtr2(out int length);
        //-------------------------------------------------



        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern int GetSimple(Simple simple);

        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern int GetSimplePtr(ref Simple simple);

        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern int GetSimpleRef(ref Simple simple);

        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern int GetSimpleIntPtr(IntPtr p);

        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern int GetSimpleIntPtrOut(out IntPtr p);
        //-------------------------------------------------


        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern int GetStudent(Student student);

        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern int GetStudentPtr(ref Student student);

        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern int GetStudentRef(ref Student student);

        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern int GetStudentIntPtr(IntPtr p);

        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern int GetStudentIntPtrOut(out IntPtr p);
        //-------------------------------------------------


    }
}
