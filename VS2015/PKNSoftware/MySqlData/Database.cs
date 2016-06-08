using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MySql.Data.MySqlClient;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Data.SqlClient;

namespace Database
{
    public class userConfigs
    {
        private Hashtable config = new Hashtable();
        public userConfigs()
        {
            readAllSetting();
        }
        public void readAllSetting()
        {
            try
            {
                var appSettings = ConfigurationManager.AppSettings;

                if (appSettings.Count == 0)
                {
                    Console.WriteLine("AppSettings is empty.");
                }
                else
                {
                    foreach (var key in appSettings.AllKeys)
                    {
                        config.Add(key, appSettings[key]);
                    }
                }
            }
            catch (ConfigurationErrorsException)
            {
                Console.WriteLine("Error reading app settings");
            }
        }
        public string getSetting(string key)
        {
            string ret;
            if (config.ContainsKey(key))
            {
                ret = config[key].ToString();
            }
            else
            {
                ret="";
            }
            return ret;
        }

        public void UpdateAllSettings(string key, string value)
        {
            try
            {
                var configFile = ConfigurationManager.OpenExeConfiguration(ConfigurationUserLevel.None);
                var settings = configFile.AppSettings.Settings;
                if (settings[key] == null)
                {
                    settings.Add(key, value);
                }
                else
                {
                    settings[key].Value = value;
                }
                configFile.Save(ConfigurationSaveMode.Modified);
                ConfigurationManager.RefreshSection(configFile.AppSettings.SectionInformation.Name);
            }
            catch (ConfigurationErrorsException)
            {
                Console.WriteLine("Error writing app settings");
            }
        }
    }

    public class MySql
    {
        private string mysqlstring;
        private userConfigs uscf = new userConfigs();
        public MySql()
        {
            mysqlstring = @"server=" + uscf.getSetting("mysqlhost") + ";user=" + uscf.getSetting("mysqlusername") + ";password=" + uscf.getSetting("mysqlpassword") + ";database=" + uscf.getSetting("mysqldatabase") + ";port=" + uscf.getSetting("mysqlport") + ";charset=utf8;";
        }
        public DataTable query(string sql)
        {
            MySqlConnection conn = new MySqlConnection(mysqlstring);
            DataSet ds = new DataSet();
            try
            {
                conn.Open();
                MySqlDataAdapter da = new MySqlDataAdapter(sql,conn);
                da.Fill(ds);
                da.Dispose();
            }
            catch (MySqlException ex)
            {
                Console.WriteLine("Error: {0}", ex.ToString());
            }
            finally
            {
                if (conn != null)
                {
                    conn.Close();
                }

            }
            return ds.Tables[0];
        }
        public void nonquery(string sql)
        {
            MySqlConnection conn = new MySqlConnection(mysqlstring);
            try
            {
                conn.Open();
                MySqlCommand cmd = new MySqlCommand();
                cmd.Connection = conn;
                cmd.CommandText = sql;
                cmd.ExecuteNonQuery();
            }
            catch (MySqlException ex)
            {
                Console.WriteLine("Error: {0}", ex.ToString());
            }
            finally
            {
                if (conn != null)
                {
                    conn.Close();
                }

            }
        }
    }
}
