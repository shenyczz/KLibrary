using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsFormsApplication1
{
    class TestMain
    {
        public static void Run()
        {
            PInvoke pi = new PInvoke();
            pi.Run();

            Radar rd = new Radar();
            rd.Run();
        }
    }
}
