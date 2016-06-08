using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Langue;
using Process;

namespace PKNSoftware
{
    public partial class Main : DevExpress.XtraBars.Ribbon.RibbonForm
    {
        private Data db = new Data();
        private Lang lang;
        private DataTable userInfo = new DataTable();
        public Main()
        {
            InitializeComponent();
            lang = new Lang(db.getUserSetting("langue"));
            this.barSubItemNhapKho.Caption = lang.get("1");
            this.barButtonNhapKhoHoaChat.Caption = lang.get("2");
            this.barButtonNhapKhoChatChuan.Caption = lang.get("3");
            this.barSubItemTieuHao.Caption = lang.get("4");
            this.barButtonTieuHaoByHoaChat.Caption = lang.get("5");
            this.barButtonTieuHaoByChiTieu.Caption = lang.get("6");
            this.barSubItemHuHongHoaChat.Caption = lang.get("7");
            this.ribbonPageHoahatChatChuan.Text = lang.get("8");
            this.ribbonPageGroupChemcal.Text = lang.get("8");
            this.ribbonPageGroupChatChuan.Text = lang.get("3");
            this.ribbonPageGroupBaoCao.Text = lang.get("9");
            this.ribbonPageThietBi.Text = lang.get("10");
            this.ribbonPageNhanSu.Text = lang.get("11");
            this.ribbonPagePhanTich.Text = lang.get("12");
            this.barButtonItem2.Caption = lang.get("13");
            this.ribbonPageGroup2.Text = lang.get("14");
            this.Text = lang.get("15");
            userInfo = db.checkLogin("nafiqad1", "nafiqad1");
            if (userInfo.Rows.Count > 0){
                MessageBox.Show("Success");
            }
            else
            {
                MessageBox.Show("Error");
            } 
            
        }
    }
}
