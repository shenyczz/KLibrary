using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Windows.Forms;

namespace WindowsFormsApplication2
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            //Radar radar = new Radar();
            //radar.Run();

            test();
        }

        void test()
        {
            string path;
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\APR\67";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\CAR\110";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\CAS\115";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\CAV\113";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\CR\37";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\CR\38";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\CRC\39";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\CS\87";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\CS\87";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\CSC\88";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\ET\41";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\ETC\42";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\HI\59";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\HSR\33";
            //path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\M\60";    //err
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\OHP\78";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\R\19";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\R\20";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\RCS\50";
            //path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\SPD\82";    //err
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\SRM\56";
            //path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\SRR\55";    //?
            //path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\SS\62";     //err
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\STI\58";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\STP\80";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\SW\30";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\SWP\47";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\THP\79";
            //path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\TVS\61"; //err
            //path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\UAM\73";     //false
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\USP\31";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\V\27";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\VAD\84";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\VCS\51";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\VIL\57";
            path = @"E:\TestData\雷达数据\雷达产品\20090603\郑州\VWP\48";

            //path = @"E:\TestData\雷达数据\雷达产品\20090603\商丘\WER\53"; //err


            DirectoryInfo folder = new DirectoryInfo(path);
            FileInfo[] fia = folder.GetFiles();
            this.listBox1.Items.AddRange(fia);

            this.listBox1.SelectedIndexChanged += listBox1_SelectedIndexChanged;

            //end
        }

        void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            FileInfo file = listBox1.SelectedItem as FileInfo;

            Console.WriteLine(file.Name);
            Console.WriteLine(file.FullName);

            Image image = null;
            bool bLoad = RadarApi.RPG_LoadData(file.FullName);

            if (bLoad)
            {
                //带有信息区的雷达图
                IntPtr intPtr = RadarApi.RPG_BuildImage(1.0f, true, true);
                image = Image.FromHbitmap(intPtr);
            }

            this.pictureBox1.Image = image;

        }
    }
}
