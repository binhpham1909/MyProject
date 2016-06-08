using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.IO.Ports;
using System.Xml;

namespace ControlPumpPlant
{
    public partial class MainForm : Form
    {
        SerialPort P = new SerialPort();
        string InputData = String.Empty;
        delegate void setTextCallBack(String text);
        Boolean InputComplete = false;
        public MainForm()
        {
            InitializeComponent();
            string[] ports = SerialPort.GetPortNames();
            cbPort.Items.AddRange(ports);
            P.ReadTimeout = 1000;
            P.DataReceived += new SerialDataReceivedEventHandler(DataReceive);
            string[] BaudRates = { "1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200" };
            cbBaudRate.Items.AddRange(BaudRates);
            string[] DataBits = { "6", "7", "8" };
            cbDataBits.Items.AddRange(DataBits);
            string[] Partys = { "None", "Odd", "Even" };
            cbParty.Items.AddRange(Partys);
            string[] stopbit = { "1", "1.5", "2" };
            cbStopBit.Items.AddRange(stopbit);
        }

        private void DataReceive(object sender, SerialDataReceivedEventArgs e)
        {
            //            throw new NotImplementedException();
            InputData = P.ReadLine();
            if (InputData != String.Empty)
            {
                InputComplete = true;
                setText(InputData);
            }
        }
        private void setText(string text)
        {
            setTextCallBack d = new setTextCallBack(setText);
        }
        private void cbPort_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (P.IsOpen)
            {
                P.Close();
            }
            P.PortName = cbPort.SelectedItem.ToString();
        }

        private void cbBaudRate_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (P.IsOpen)
            {
                P.Close();
            }
            P.BaudRate = Convert.ToInt32(cbBaudRate.Text);
        }

        private void cbDataBits_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (P.IsOpen)
            {
                P.Close();
            }
            P.DataBits = Convert.ToInt32(cbDataBits.Text);
        }

        private void cbParty_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (P.IsOpen)
            {
                P.Close();
            }
            switch (cbParty.SelectedItem.ToString())
            {
                case "Odd":
                    P.Parity = Parity.Odd;
                    break;
                case "None":
                    P.Parity = Parity.None;
                    break;
                case "Even":
                    P.Parity = Parity.Even;
                    break;
            }

        }

        private void cbStopBit_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (P.IsOpen)
            {
                P.Close();
            }
            switch (cbStopBit.SelectedItem.ToString())
            {
                case "1":
                    P.StopBits = StopBits.One;
                    break;
                case "1.5":
                    P.StopBits = StopBits.OnePointFive;
                    break;
                case "2":
                    P.StopBits = StopBits.Two;
                    break;
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            //cbPort.SelectedIndex = 0;   // Cong com dau tien
            cbBaudRate.SelectedIndex = 7;   // 115200
            cbDataBits.SelectedIndex = 2;   // 8
            cbParty.SelectedIndex = 0;  //none
            cbStopBit.SelectedIndex = 0;    // none

        }

        private void btnKetNoi_Click(object sender, EventArgs e)
        {
            try
            {
                P.Open();
                btnKetNoi.Enabled = false;
                btnNgatKetNoi.Enabled = true;
                status.Text = "Đang kết nối" + cbPort.SelectedItem.ToString();
            }
            catch(Exception ex)
            {
                MessageBox.Show("Không kết nối được", "Thử lại", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnNgatKetNoi_Click(object sender, EventArgs e)
        {
            P.Close();
            btnKetNoi.Enabled = true;
            btnNgatKetNoi.Enabled = false;
            status.Text = "Đã ngắt kết nối";
        }

        private void btnExit_Click(object sender, EventArgs e)
        {
            DialogResult kq = MessageBox.Show("Thoát khỏi chương trình điều khiển", "HBInvent.vn", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if(kq == DialogResult.Yes)
            {
                if (P.IsOpen)
                {
                    P.Close();
                }
                MessageBox.Show("Cảm ơn bạn đã sử dụng chương trình");
                this.Close();
            }
        }

        private void lbAbout_Click(object sender, EventArgs e)
        {
            About frm = new About();
            frm.ShowDialog();
        }

        private void btnReadDV_Click(object sender, EventArgs e)
        {
            if (P.IsOpen)
            {
                InputComplete = false;
                proBar.Value = 0;
                P.WriteLine(">1:");
                while (!InputComplete) ;
                proBar.Value = 100;
                MessageBox.Show(InputData);
            }
        }
    }
}
