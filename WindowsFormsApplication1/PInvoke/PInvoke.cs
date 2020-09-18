using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{

    public class PInvoke
    {
        public void Run()
        {
            pinvoke_value();

            //pinvoke_string();

            //pinvoke_array();

            //pinvoke_Simple();

            //pinvoke_Student();

            //pinvoke_Group();

        }

        void pinvoke_value()
        {
            int iv = PInvokeApi.GetValue(5);
            Console.WriteLine("API库调用正常。PInvokeApi.GetValue = {0}", iv);

            return;
        }

        void pinvoke_string()
        {
            IntPtr intPtr = PInvokeApi.GetString("123456-ABC.abc~_中文");
            String strOut = Marshal.PtrToStringAuto(intPtr);
            //end
        }

        void pinvoke_array()
        {
            //v*2
            int[] ary1 = new int[] { 1, 2, 3, 4, 5, 6 };
            PInvokeApi.GetArray(ary1, ary1.Length);

            //v*3
            int[] ary2 = new int[] { 1, 2, 3, 4, 5, 6 };
            PInvokeApi.GetArrayPtr(ary2, ary2.Length);

            //v*4
            int[] ary3 = new int[] { 1, 2, 3, 4, 5, 6 };
            PInvokeApi.GetArrayRef(ary3, ary3.Length);

            //---
            int len;    //out
            IntPtr p;   //out
            PInvokeApi.GetArrayDoubleIntPtr(out p, out len);
            double[] ary4 = new double[len];
            Marshal.Copy(p, ary4, 0, len);

            //---
            int len2;   //out
            IntPtr p2 = PInvokeApi.GetArrayDoubleIntPtr2(out len2);
            double[] ary42 = new double[len2];
            Marshal.Copy(p2, ary42, 0, len2);

            //end
        }

        void pinvoke_Simple()
        {
            int size = Marshal.SizeOf(typeof(Simple));

            //41
            Simple simple41 = new Simple();
            PInvokeApi.GetSimple(simple41);

            //42
            Simple simple42 = new Simple();
            PInvokeApi.GetSimplePtr(ref simple42);

            //43
            Simple simple43 = new Simple();
            PInvokeApi.GetSimpleRef(ref simple43);

            //44
            IntPtr p44 = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(Simple)));
            PInvokeApi.GetSimpleIntPtr(p44);
            Simple simple44 = (Simple)Marshal.PtrToStructure(p44, typeof(Simple));
            Marshal.FreeHGlobal(p44);

            //45
            IntPtr p45;
            PInvokeApi.GetSimpleIntPtrOut(out p45);
            Simple simple45 = (Simple)Marshal.PtrToStructure(p45, typeof(Simple));

            //end
        }

        void pinvoke_Student()
        {
            int size = Marshal.SizeOf(typeof(Student));

            //51
            Student student51 = new Student();
            PInvokeApi.GetStudent(student51);

            //52
            Student student52 = new Student();
            PInvokeApi.GetStudentPtr(ref student52);

            //53
            Student student53 = new Student();
            PInvokeApi.GetStudentRef(ref student53);

            //54
            IntPtr p54 = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(Student)));
            PInvokeApi.GetStudentIntPtr(p54);
            Student student54 = (Student)Marshal.PtrToStructure(p54, typeof(Student));
            Marshal.FreeHGlobal(p54);

            //55
            IntPtr p55;
            PInvokeApi.GetStudentIntPtrOut(out p55);
            Student student55 = (Student)Marshal.PtrToStructure(p55, typeof(Student));

            //end
        }
    }

}
