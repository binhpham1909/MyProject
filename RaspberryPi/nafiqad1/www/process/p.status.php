<?php
require("../config/config.php");
session_start();
if ($_SESSION['user'])
{
$user_id=$_SESSION['user_id']; 

$postdata = file_get_contents("php://input");
$request = json_decode($postdata);

$data['action']=$request->action;
$data['id']=$request->id;
$data['name']=$request->name;
$data['type']=$request->type;
$data['new_name']=$request->new_name;
$data['properties']=$request->properties;

$reply=array();

if($data['action']=='add'){
	$sql="INSERT INTO pkn_chemical (name,properties,type) VALUE ('{$data['name']}','{$data['properties']}','{$data['type']}')";
	if(mysql_query ($sql)){
		$reply['result']="ok";
		$reply['message']="Thêm thành công";
	}else{
		$reply['result']="err";
		$reply['message']="Thêm thất bại";
	}
}elseif($data['action']=='edit'){
	$sql="UPDATE pkn_chemical SET name='{$data['new_name']}',properties='{$data['properties']}',type='{$data['type']}' WHERE id='{$data['id']}'";
	if(mysql_query ($sql)){
		$reply['result']="ok";
		$reply['message']="Sửa thành công";
	}else{
		$reply['result']="err";
		$reply['message']="Sửa thất bại";
	}
}elseif($data['action']=='del'){
	$sql="DELETE FROM pkn_chemical WHERE id='{$data['id']}'";
	if(mysql_query ($sql)){
		$reply['result']="ok";
		$reply['message']="Xóa thành công";
	}else{
		$reply['result']="err";
		$reply['message']="Xóa thất bại";
	}
}
echo json_encode($reply);
}
?>