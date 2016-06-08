using System.Globalization;
using System.Resources;
namespace Langue
{
    public class Lang
    {
        private ResourceManager langue;    // declare Resource manager to access to specific cultureinfo
        private CultureInfo cul;            // declare culture info
        public Lang(string setup)
        {
            //   Assembly resourceAssembly = Assembly.LoadFrom("Langue.dll");
            langue = new ResourceManager("Langue.View", System.Reflection.Assembly.LoadFrom("Langue.dll"));
            cul = CultureInfo.CreateSpecificCulture(setup);
        }
        public string get(string name)
        {
            return langue.GetString(name, cul);
        }
    }
}
