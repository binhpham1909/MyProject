<?php
require("../config/config.php");
 
$postdata = file_get_contents("php://input");
$request = json_decode($postdata);
$type=$request->type;
$id=$request->id;
$min=$request->min;
$max=$request->max;
$reply;
$sql1="";
if($type=='humi'){
	if(UpdateHumiRange($id,$min,$max)){
		$reply=array("result"=>"Đổi thành công");
	}else{
		$reply=array("result"=>"Đổi thất bại");
	}	
}elseif($type=='temp'){
	if(UpdateTempRange($id,$min,$max)){
		$reply=array("result"=>"Đổi thành công");
	}else{
		$reply=array("result"=>"Đổi thất bại");
	}		
}

echo json_encode($reply);
?>