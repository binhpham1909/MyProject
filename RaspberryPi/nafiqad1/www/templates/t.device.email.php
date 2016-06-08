<?php 
require("../config/lang.ini.php");
require("../config/config.php"); 
session_start();
if (!$_SESSION['user'])
{ 
?>
<ion-view view-title="<?php echo __list_receive_email?>" name="device_email-tab">
	<ion-content class="padding">
    <?php echo __you_dont_permission?>
    </ion-content>
</ion-view>
<?php
} else{
?>
<ion-view  view-title="<?php echo __list_receive_email?>" name="device_email-tab">
<ion-nav-buttons side="right">
	<button class="button button-positive button-clear icon-left ion-plus" ng-click="show.del=false;show.edit=false;add()"><?php echo __add_receive_sms?></button>
	<button class="button button-balanced button-clear icon-left ion-edit" ng-click="show.edit=!show.edit;show.del=false"><?php echo __edit_receive_sms?></button>
	<button class="button button-assertive button-clear icon-left ion-ios-close-outline" ng-click="show.del=!show.del;show.edit=false"><?php echo __del_receive_sms?></button>
</ion-nav-buttons>
<ion-content class="padding">
<ion-list show-delete="show.del" show-reorder="show.edit">
	<ion-item class="item-remove-animate" ng-repeat="user in users">
	{{user.name}} - {{user.email}}
	<ion-delete-button class="ion-ios-minus-outline balanced" ng-click="del(user.id,user.name,user.email)"></ion-delete-button>
	<ion-reorder-button class="ion-edit" ng-click="edit(user.id,user.name)"></ion-reorder-button>
	</ion-item>
</ion-list>
</ion-content>
</ion-view>
<?php }?>