namespace PKNSoftware
{
    partial class xemHoaChat
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            DevExpress.DataAccess.Sql.TableQuery tableQuery4 = new DevExpress.DataAccess.Sql.TableQuery();
            DevExpress.DataAccess.Sql.RelationInfo relationInfo16 = new DevExpress.DataAccess.Sql.RelationInfo();
            DevExpress.DataAccess.Sql.RelationColumnInfo relationColumnInfo16 = new DevExpress.DataAccess.Sql.RelationColumnInfo();
            DevExpress.DataAccess.Sql.RelationInfo relationInfo17 = new DevExpress.DataAccess.Sql.RelationInfo();
            DevExpress.DataAccess.Sql.RelationColumnInfo relationColumnInfo17 = new DevExpress.DataAccess.Sql.RelationColumnInfo();
            DevExpress.DataAccess.Sql.RelationInfo relationInfo18 = new DevExpress.DataAccess.Sql.RelationInfo();
            DevExpress.DataAccess.Sql.RelationColumnInfo relationColumnInfo18 = new DevExpress.DataAccess.Sql.RelationColumnInfo();
            DevExpress.DataAccess.Sql.RelationInfo relationInfo19 = new DevExpress.DataAccess.Sql.RelationInfo();
            DevExpress.DataAccess.Sql.RelationColumnInfo relationColumnInfo19 = new DevExpress.DataAccess.Sql.RelationColumnInfo();
            DevExpress.DataAccess.Sql.RelationInfo relationInfo20 = new DevExpress.DataAccess.Sql.RelationInfo();
            DevExpress.DataAccess.Sql.RelationColumnInfo relationColumnInfo20 = new DevExpress.DataAccess.Sql.RelationColumnInfo();
            DevExpress.DataAccess.Sql.TableInfo tableInfo19 = new DevExpress.DataAccess.Sql.TableInfo();
            DevExpress.DataAccess.Sql.ColumnInfo columnInfo34 = new DevExpress.DataAccess.Sql.ColumnInfo();
            DevExpress.DataAccess.Sql.TableInfo tableInfo20 = new DevExpress.DataAccess.Sql.TableInfo();
            DevExpress.DataAccess.Sql.ColumnInfo columnInfo35 = new DevExpress.DataAccess.Sql.ColumnInfo();
            DevExpress.DataAccess.Sql.ColumnInfo columnInfo36 = new DevExpress.DataAccess.Sql.ColumnInfo();
            DevExpress.DataAccess.Sql.ColumnInfo columnInfo37 = new DevExpress.DataAccess.Sql.ColumnInfo();
            DevExpress.DataAccess.Sql.TableInfo tableInfo21 = new DevExpress.DataAccess.Sql.TableInfo();
            DevExpress.DataAccess.Sql.ColumnInfo columnInfo38 = new DevExpress.DataAccess.Sql.ColumnInfo();
            DevExpress.DataAccess.Sql.TableInfo tableInfo22 = new DevExpress.DataAccess.Sql.TableInfo();
            DevExpress.DataAccess.Sql.ColumnInfo columnInfo39 = new DevExpress.DataAccess.Sql.ColumnInfo();
            DevExpress.DataAccess.Sql.ColumnInfo columnInfo40 = new DevExpress.DataAccess.Sql.ColumnInfo();
            DevExpress.DataAccess.Sql.TableInfo tableInfo23 = new DevExpress.DataAccess.Sql.TableInfo();
            DevExpress.DataAccess.Sql.ColumnInfo columnInfo41 = new DevExpress.DataAccess.Sql.ColumnInfo();
            DevExpress.DataAccess.Sql.ColumnInfo columnInfo42 = new DevExpress.DataAccess.Sql.ColumnInfo();
            DevExpress.DataAccess.Sql.TableInfo tableInfo24 = new DevExpress.DataAccess.Sql.TableInfo();
            DevExpress.DataAccess.Sql.ColumnInfo columnInfo43 = new DevExpress.DataAccess.Sql.ColumnInfo();
            DevExpress.DataAccess.Sql.ColumnInfo columnInfo44 = new DevExpress.DataAccess.Sql.ColumnInfo();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(xemHoaChat));
            this.sqlDataSource1 = new DevExpress.DataAccess.Sql.SqlDataSource(this.components);
            this.SuspendLayout();
            // 
            // sqlDataSource1
            // 
            this.sqlDataSource1.ConnectionName = "BINHPHAM-PC\\SQLEXPRESS_pkn_Connection";
            this.sqlDataSource1.Name = "sqlDataSource1";
            tableQuery4.Name = "pkn_tenHoaChat";
            relationColumnInfo16.NestedKeyColumn = "IDhoaChat";
            relationColumnInfo16.ParentKeyColumn = "IDhoaChat";
            relationInfo16.KeyColumns.AddRange(new DevExpress.DataAccess.Sql.RelationColumnInfo[] {
            relationColumnInfo16});
            relationInfo16.NestedTable = "pkn_hoaChat";
            relationInfo16.ParentTable = "pkn_tenHoaChat";
            relationColumnInfo17.NestedKeyColumn = "IDhoaChat";
            relationColumnInfo17.ParentKeyColumn = "IDhoaChat";
            relationInfo17.KeyColumns.AddRange(new DevExpress.DataAccess.Sql.RelationColumnInfo[] {
            relationColumnInfo17});
            relationInfo17.NestedTable = "pkn_kieuHoaChat";
            relationInfo17.ParentTable = "pkn_hoaChat";
            relationInfo18.JoinType = DevExpress.Xpo.DB.JoinType.LeftOuter;
            relationColumnInfo18.NestedKeyColumn = "IDkieuHoaChat";
            relationColumnInfo18.ParentKeyColumn = "IDkieuHoaChat";
            relationInfo18.KeyColumns.AddRange(new DevExpress.DataAccess.Sql.RelationColumnInfo[] {
            relationColumnInfo18});
            relationInfo18.NestedTable = "pkn_hoaChatNhap";
            relationInfo18.ParentTable = "pkn_kieuHoaChat";
            relationColumnInfo19.NestedKeyColumn = "unitID";
            relationColumnInfo19.ParentKeyColumn = "IDdonVi";
            relationInfo19.KeyColumns.AddRange(new DevExpress.DataAccess.Sql.RelationColumnInfo[] {
            relationColumnInfo19});
            relationInfo19.NestedTable = "pkn_unit";
            relationInfo19.ParentTable = "pkn_hoaChatNhap";
            relationColumnInfo20.NestedKeyColumn = "userID";
            relationColumnInfo20.ParentKeyColumn = "IDnguoiNhap";
            relationInfo20.KeyColumns.AddRange(new DevExpress.DataAccess.Sql.RelationColumnInfo[] {
            relationColumnInfo20});
            relationInfo20.NestedTable = "pkn_user";
            relationInfo20.ParentTable = "pkn_hoaChatNhap";
            tableQuery4.Relations.AddRange(new DevExpress.DataAccess.Sql.RelationInfo[] {
            relationInfo16,
            relationInfo17,
            relationInfo18,
            relationInfo19,
            relationInfo20});
            tableInfo19.Name = "pkn_tenHoaChat";
            columnInfo34.Name = "tenHoaChat";
            tableInfo19.SelectedColumns.AddRange(new DevExpress.DataAccess.Sql.ColumnInfo[] {
            columnInfo34});
            tableInfo20.Name = "pkn_hoaChat";
            columnInfo35.Name = "thongTinHoaChat";
            columnInfo36.Name = "hoaChatLeft";
            columnInfo37.Name = "hoaChatRight";
            tableInfo20.SelectedColumns.AddRange(new DevExpress.DataAccess.Sql.ColumnInfo[] {
            columnInfo35,
            columnInfo36,
            columnInfo37});
            tableInfo21.Name = "pkn_kieuHoaChat";
            columnInfo38.Name = "tenKieuHoaChat";
            tableInfo21.SelectedColumns.AddRange(new DevExpress.DataAccess.Sql.ColumnInfo[] {
            columnInfo38});
            tableInfo22.Name = "pkn_hoaChatNhap";
            columnInfo39.Name = "ngayNhapHoaChat";
            columnInfo40.Name = "luongNhapHoaChat";
            tableInfo22.SelectedColumns.AddRange(new DevExpress.DataAccess.Sql.ColumnInfo[] {
            columnInfo39,
            columnInfo40});
            tableInfo23.Name = "pkn_unit";
            columnInfo41.Name = "unitSymbol";
            columnInfo42.Name = "unitName";
            tableInfo23.SelectedColumns.AddRange(new DevExpress.DataAccess.Sql.ColumnInfo[] {
            columnInfo41,
            columnInfo42});
            tableInfo24.Name = "pkn_user";
            columnInfo43.Name = "userName";
            columnInfo44.Name = "userFullName";
            tableInfo24.SelectedColumns.AddRange(new DevExpress.DataAccess.Sql.ColumnInfo[] {
            columnInfo43,
            columnInfo44});
            tableQuery4.Tables.AddRange(new DevExpress.DataAccess.Sql.TableInfo[] {
            tableInfo19,
            tableInfo20,
            tableInfo21,
            tableInfo22,
            tableInfo23,
            tableInfo24});
            this.sqlDataSource1.Queries.AddRange(new DevExpress.DataAccess.Sql.SqlQuery[] {
            tableQuery4});
            this.sqlDataSource1.ResultSchemaSerializable = resources.GetString("sqlDataSource1.ResultSchemaSerializable");
            // 
            // xemHoaChat
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1049, 571);
            this.Name = "xemHoaChat";
            this.Text = "xemHoaChat";
            this.ResumeLayout(false);

        }

        #endregion
        private DevExpress.DataAccess.Sql.SqlDataSource sqlDataSource1;
    }
}