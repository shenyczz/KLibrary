using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;


namespace WindowsFormsApplication1
{
    public class Radar
    {
        public void Run()
        {
            radar_test();
        }


        void radar_test()
        {
            Image image = null;
            IntPtr intPtr = IntPtr.Zero;
            string fn = "TestData\\20090603.154752.01.19.371";
            string fnImageB = @"TestData\20090603.154752.01.19.371.buil.png";
            string fnImageC = @"TestData\20090603.154752.01.19.371.colr.png";
            string fnImageG = @"TestData\20090603.154752.01.19.371.gray.png";

            bool bLoad = RadarApi.RPG_LoadData(fn);

            if (bLoad)
            {
                //带有信息区的雷达图
                intPtr = RadarApi.RPG_BuildImage(1.0f, true, true);
                image = Image.FromHbitmap(intPtr);
                image.Save(fnImageB);

                //分析用的雷达图
                intPtr = RadarApi.RPG_ToImageColor();
                image = Image.FromHbitmap(intPtr);
                image.Save(fnImageC);

                //根据反射率数据生成的灰度图
                //可作为高程图使用
                intPtr = RadarApi.RPG_ToImageGray();
                image = Image.FromHbitmap(intPtr);
                image.Save(fnImageG);

                //雷达产品格点数据
                IntPtr p;
                RadarGridInfo rgi;
                int n = RadarApi.RPG_ToGrid(out p, out rgi);
                int col = rgi.xNum;
                int row = rgi.yNum;
                double[] rad = new double[col * row];
                Marshal.Copy(p, rad, 0, col * row);

                //end
            }

        }
    }

}
