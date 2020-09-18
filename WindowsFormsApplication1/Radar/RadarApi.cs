using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace WindowsFormsApplication1
{
    public class RadarApi : TestApi
    {

        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern bool RPG_LoadData(String filename);

        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern IntPtr RPG_BuildImage(float fZoom, bool bDrawCoordinate, bool bDrawInfoArea);

        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern IntPtr RPG_ToImageColor();

        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern IntPtr RPG_ToImageGray();

        [DllImport(dll_name, CharSet = CharSet.Auto)]
        public static extern int RPG_ToGrid(out IntPtr p, out RadarGridInfo radarGridInfo);

    }
}
