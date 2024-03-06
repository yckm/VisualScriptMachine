namespace WinFormsApp
{
    internal static class Program
    {
        /// <summary>
        ///  The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Sm.InitSm();
            ApplicationConfiguration.Initialize();
            Application.Run(new Home());
        }
    }
}