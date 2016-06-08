<?php 
require("../config/lang.ini.php");
require("../config/config.php"); 
?>
<ion-view view-title="<?php echo __temp?>" name="status_temp-tab">
<ion-content class="padding">
	<ion-list>
		<ion-item class="item item-icon-left" ng-repeat="temp in temps">
			<i class="icon ion-thermometer"></i>
			<h2>{{temp.name}}</h2> <?php echo __limit_min?> : {{temp.min_temp}} - <?php echo __limit_max?> : {{temp.max_temp}}
			<span class="badge badge-assertive">{{temp.temp}}</span>
		</ion-item>
	</ion-list>
</ion-content>
</ion-view>