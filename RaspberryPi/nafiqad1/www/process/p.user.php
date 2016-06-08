<?php
require("../config/config.php");
require("../config/lang.ini.php");
$postdata = file_get_contents("php://input");
$request = json_decode($postdata);
$action=$request->action;
$content=$request->content;
$name=$request->name;
$value=$request->value;
$id=$request->id;
$reply;
$sql1="";
if($content=='sms'){
	if($action=='read'){
		$reply=ReadUserReceiveSms();
	}elseif($action=='edit'){
		if(EditUserReceiveSms($id,$name,$value)){
			$reply=array("result"=>__edit_success);
		}else{
			$reply=array("result"=>__edit_err);
		}	
	}elseif($action=='add'){
		if(AddUserReceiveSms($name,$value)){
			$reply=array("result"=>__add_success);
		}else{
			$reply=array("result"=>__add_err);
		}	
	}elseif($action=='del'){
		if(DelUserReceiveSms($id)){
			$reply=array("result"=>__del_success);
		}else{
			$reply=array("result"=>__del_err);
		}	
	}
}elseif($content=='email'){
	if($action=='read'){
		$reply=ReadUserReceiveEmail();
	}elseif($action=='edit'){
		if(EditUserReceiveEmail($id,$name,$value)){
			$reply=array("result"=>__edit_success);
		}else{
			$reply=array("result"=>__edit_err);
		}	
	}elseif($action=='add'){
		if(AddUserReceiveEmail($name,$value)){
			$reply=array("result"=>__add_success);
		}else{
			$reply=array("result"=>__add_err);
		}	
	}elseif($action=='del'){
		if(DelUserReceiveEmail($id)){
			$reply=array("result"=>__del_success);
		}else{
			$reply=array("result"=>__del_err);
		}	
	}
}

echo json_encode($reply);
?>