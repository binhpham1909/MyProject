<?php 
require("../config/lang.ini.php");
require("../config/config.php"); 
?>
<ion-view view-title="<?php echo __humi?>" name="status_humi-tab">
<ion-content class="padding">
	<ion-list>
        <ion-item ng-repeat="humi in humis" class="item item-icon-left">
		<i class="icon ion-umbrella"></i>
		<h2>{{humi.name}}</h2> <?php echo __limit_min?> : {{humi.min_humi}} - <?php echo __limit_max?> : {{humi.max_humi}}
		<span class="badge badge-assertive">{{humi.humi}}</span>
		</ion-item>
	</ion-list>
</ion-content>
</ion-view>