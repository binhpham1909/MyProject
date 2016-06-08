using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Database;
using System.Collections;
using System.Data;

namespace Process
{
    public class Data
    {
        private userConfigs cf = new userConfigs();
        private MySql mydb = new MySql();
        public Data()
        {   
            
        }
        public string getUserSetting(string k)
        {
            return cf.getSetting(k);
        }
        public void setupDatabase(string host, string port, string username, string password, string database)
        {
            cf.UpdateAllSettings("mysqlhost", host);
            cf.UpdateAllSettings("mysqlport", port);
            cf.UpdateAllSettings("mysqlusername", username);
            cf.UpdateAllSettings("mysqlpassword", password);
            cf.UpdateAllSettings("mysqldatabase", database);
        }
        DataTable checkLogin(string username, string password)
        {
            DataTable tb = mydb.query(@"select * from pkn_user where username='" + username + "' AND password='" + password + "'");
//            if (tb.Rows.Count > 0) return true;
            return tb;
        }
    }
}
