<?php 
require("../config/lang.ini.php");
require("../config/config.php"); 
?>
<ion-view view-title="<?php echo __status?>" name="status-view">
	<ion-content class="padding">
  	<ion-list class="">
		<ion-item class="item-icon-left" href="#/tab/status.temp"><i class="icon ion-thermometer"></i><?php echo __temp?></ion-item>
		<ion-item class="item-icon-left" href="#/tab/status.humi"><i class="icon ion-umbrella"></i><?php echo __humi?></ion-item>
		<ion-item class="item-toggle item-icon-left">
			<i class="icon ion-android-notifications"></i>
			<?php echo __ring?>
			<label class="toggle toggle-assertive">
			<input type="checkbox" ng-model="bell.checked" ng-change="bellchange()">
			<div class="track"><div class="handle"></div></div>
			</label>
		</ion-item>
		<ion-item class="item-icon-left" href="login.tpl.php"><i class="icon ion-ios-unlocked"></i><?php echo __login?></ion-item>
	<ion-list>
  </ion-content>
</ion-view>