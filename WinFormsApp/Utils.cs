using System.Runtime.InteropServices;

namespace WinFormsApp
{
    public static class Utils
    {
        public static string C2Str(IntPtr ptr)
        {
            if (ptr == IntPtr.Zero)
            {
                return "";
            }
            return Marshal.PtrToStringUTF8(ptr);
        }

        /// <summary>
        /// 检查 double变量是否为0 
        /// </summary>
        /// <param name="x"> 需要判断的值,默认在0左右1e-10内即为0</param>
        /// <returns></returns>
        public static bool IsZero(this double x)
        {
            return x < 1e-10 && x > -1e-10;
        }

    }
}
