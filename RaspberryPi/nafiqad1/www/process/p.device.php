<?php
require("../config/config.php");
require("../config/lang.ini.php");
$postdata = file_get_contents("php://input");
$request = json_decode($postdata);
$action=$request->action;
$nameid=$request->nameid;
$newname=$request->newname;
$addname=$request->name;
$serial=$request->serial;
$reply;
$sql1="";
if($action=='rename'){
	if(RenameDevice($nameid,$newname)){
		$reply=array("result"=>"Đổi tên thành công");
	}else{
		$reply=array("result"=>"Đổi tên thất bại");
	}	
}elseif($action=='add'){
	if(substr($serial,0,3)=="THS"){
		if(!CheckExistDeviceSerial($serial)){
			NewDevice($serial,$addname);
			NewHumiSensor(GetDeviceIdFromSerial($serial));
			NewTempSensor(GetDeviceIdFromSerial($serial));
			$reply=array("result"=>__add_success);
		}else{
			$reply=array("result"=>__exist_device);
		}
	}else{
		$reply=array("result"=>__serial_wrong);
	}
}

echo json_encode($reply);
?>