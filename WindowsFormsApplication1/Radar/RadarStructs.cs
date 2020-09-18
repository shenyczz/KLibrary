using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace WindowsFormsApplication1
{
    [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Auto)]
    public struct RadarGridInfo
    {
        //x分量
        public Int16 xNum;			//X方向格点数(列数)
        public double xMin;			//X坐标最小值
        public double xMax;			//X坐标最大值
        public double xInterval;    //X坐标间隔值

        //y分量
        public Int16 yNum;			//Y方向格点数(行数)
        public double yMin;			//Y坐标最小值
        public double yMax;			//Y坐标最大值
        public double yInterval;	//Y坐标间隔值

        //时间
        public UInt16 wYear;		//年
        public UInt16 wMonth;		//月
        public UInt16 wDay;			//日
        public UInt16 wHour;		//时
        public UInt16 wMinute;		//分
        public UInt16 wSecond;		//秒

    };

}
