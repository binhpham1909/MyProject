myapp
.controller('status', function($scope,$ionicPopup,DB,$timeout) {
	$scope.bell = { checked: true };
	$scope.bellchange = function() {
		var confirmPopup = $ionicPopup.confirm({title: 'Xác nhận',template: 'Bạn muốn thực hiện hành động này?'});
		confirmPopup.then(function(res) {
			if(res) {
				$timeout(function() {
					DB.SetConfig("bell",$scope.bell.checked).then(function(result) {
						$ionicPopup.alert({
							title: 'Kết quả',
							template: result.data.result
						});
						ReadConfig();
					},function(err) {console.log(err);});
					});
			} else{
				ReadConfig();
		   }
		});
	};
	var ReadConfig=function(){
		DB.ReadConfig().then(function(result) {
			if(result.data.bell=='1') $scope.bell.checked=true; else $scope.bell.checked=false;
			},function(err) {console.log(err);}
		);		
	}	
})
.controller('status_temp', function($scope,$ionicPopup,$interval,DB) {
	$scope.temps={};
	gettemp=function(){
		DB.gettemp().then(function(result) {
			$scope.temps=result.data;
		},function(err) {
			console.log(err);
		});
	};
	$interval(gettemp, 1000);
})
.controller('status_humi', function($scope,$ionicPopup,$interval,DB) {
	$scope.humis={};
	$scope.showEdit=false;
	gethumi=function(){
		DB.gethumi().then(function(result) {
			$scope.humis=result.data;
		},function(err) {
			console.log(err);
		});
	};
	$interval(gethumi, 1000);
})
