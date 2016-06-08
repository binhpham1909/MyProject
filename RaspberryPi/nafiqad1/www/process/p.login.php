<?php 
require_once("../config/config.php"); 
require_once("../config/lang.ini.php");
header('Content-Type: text/html; charset=UTF-8');
session_start();
$postdata = file_get_contents("php://input");
$request = json_decode($postdata);

$action=$request->action;
$username=$request->username;
$password=$request->password;
$reply=array();
if ($action == "login")
{
    // Dùng hàm addslashes() để tránh SQL injection, dùng hàm md5() để mã hóa password
    $username = addslashes( $username );
    $password = md5( addslashes( $password ) );
    // Lấy thông tin của username đã nhập trong table members
    $sql_query = @mysql_query("SELECT * FROM user WHERE username='{$username}' AND password='{$password}'");
    $member = @mysql_fetch_array( $sql_query );
    // Nếu username này không tồn tại thì....
    if ( @mysql_num_rows( $sql_query ) <= 0 )
    {
		$reply['result']="err";
		$reply['message']=__invalid_user_password;
    }else{
		    // Khởi động phiên làm việc (session)
		$_SESSION['user_id'] = $member['id'];
		$_SESSION['user'] = $member['username'];
///		$_SESSION['admin'] = $member['admin'];
		$reply['result']="ok";
		$reply['message']=__login_success;
	}
    // Thông báo đăng nhập thành công
}else if( $action == "logout"){
	if (session_destroy()){
		$reply['result']="ok";
		$reply['message']=__logout_success;		
	}
	else{
		$reply['result']="err";
		$reply['message']=__logout_err;		
	}
}
echo json_encode($reply);
?>