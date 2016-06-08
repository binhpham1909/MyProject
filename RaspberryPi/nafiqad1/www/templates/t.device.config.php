<?php 
require("../config/lang.ini.php");
require("../config/config.php"); 
session_start();
if (!$_SESSION['user'])
{ 
?>
<ion-view view-title="<?php echo __config_device?>" name="device_config-tab">
	<ion-content class="padding">
    <?php echo __you_dont_permission?>
    </ion-content>
</ion-view>
<?php
} else{
?>
<ion-view  view-title="<?php echo __config_device?>" name="device_config-tab">
<ion-content class="padding">
<ion-list class="">
	<ion-item class="item-toggle item-icon-left">
		<i class="icon ion-android-warning"></i>
		<?php echo __alert_alert?>
		<label class="toggle toggle-assertive">
		<input type="checkbox" ng-model="alerts.checked" ng-change="alertschange()">
		<div class="track"><div class="handle"></div></div>
		</label>
	</ion-item>
	<ion-item class="item-toggle item-icon-left">
		<i class="icon ion-android-notifications"></i>
		<?php echo __ring_alert?>
		<label class="toggle toggle-assertive">
		<input type="checkbox" ng-model="ring.checked" ng-change="ringchange()">
		<div class="track"><div class="handle"></div></div>
		</label>
	</ion-item>
	<ion-item class="item-toggle item-icon-left">
		<i class="icon ion-ios-chatboxes"></i>
		<?php echo __sms_alert?>
		<label class="toggle toggle-assertive">
		<input type="checkbox" ng-model="sms.checked" ng-change="smschange()">
		<div class="track"><div class="handle"></div></div>
		</label>
	</ion-item>
	<ion-item class="item-toggle item-icon-left">
		<i class="icon ion-ios-email"></i>
		<?php echo __email_alert?>
		<label class="toggle toggle-assertive">
		<input type="checkbox" ng-model="email.checked" ng-change="emailchange()">
		<div class="track"><div class="handle"></div></div>
		</label>
	</ion-item>
	<ion-item class="item-icon-left" href="#/tab/device.sms"><i class="icon ion-ios-chatboxes-outline"></i><?php echo __list_receive_sms?></ion-item>
	<ion-item class="item-icon-left" href="#/tab/device.email"><i class="icon ion-ios-email-outline"></i><?php echo __list_receive_email?></ion-item>
</ion-list>
</ion-content>
</ion-view>
<?php }?>