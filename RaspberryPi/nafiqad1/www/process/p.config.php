<?php
require("../config/config.php");
require("../config/lang.ini.php");
$postdata = file_get_contents("php://input");
$request = json_decode($postdata);
$action=$request->action;
$key=$request->key;
$value=$request->value;

$reply;
$sql1="";
if($action=='edit'){
	if($value==true){$value=1;}else{$value=0;}
	if($key=='alert'){
		if((SetConfig('alert',$value))&&SetConfig('ring',$value)&&SetConfig('sms',$value)&&SetConfig('email',$value)){
			$reply=array("result"=>"Thiết lập thành công");
		}else{
			$reply=array("result"=>"Thiết lập thất bại");
		}
	}else{
		if(SetConfig($key,$value)){
			$reply=array("result"=>"Thiết lập thành công");
		}else{
			$reply=array("result"=>"Thiết lập thất bại");
		}		
	}
}elseif($action=='read'){
	$reply=ReadConfig();
}
echo json_encode($reply);
?>