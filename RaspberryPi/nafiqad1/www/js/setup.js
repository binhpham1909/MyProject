myapp
.controller('setup', function($scope,$ionicHistory,$ionicPopup,CACHE) {
})
.controller('setup_temp', function($scope,$ionicPopup,DB) {
	$scope.temps={};
	$scope.showEdit=false;
	$scope.Edit=function(){
		$scope.showEdit=!$scope.showEdit;
		$scope.showRename=false;
	};
	$scope.visRename=function(){
		$scope.showRename=!$scope.showRename;
		$scope.showEdit=false;
	};
	$scope.rename=function(id,name){
		$scope.tosend={};
		$scope.tosend.id=id;
		var myPopup = $ionicPopup.show({
			template: '<input type="text" ng-model="tosend.newname">',
			title: name,
			subTitle: 'Nhập tên mới',
			scope: $scope,
			buttons: [{ text: 'Cancel' },{text: '<b>Save</b>',type: 'button-positive',
				onTap: function(e) {
					if (!$scope.tosend.newname) {
						e.preventDefault();
					} else {
						return $scope.tosend;
					}
				}
			},]
		});
		myPopup.then(function(res) {
			DB.RenameDevice(res.id,res.newname).then(function(result) {
				$ionicPopup.alert({
					title: 'Kết quả',
					template: result.data.result
				});
			},function(err) {console.log(err);}
			);
		});
	}
	$scope.changerange=function(id,name){
		$scope.tosend={};
		$scope.tosend.id=id;
		$scope.tosend.type='temp';
		var myPopup = $ionicPopup.show({
			template: '<input type="number" placeholder="Ngưỡng dưới" ng-model="tosend.newmin"><input type="number" placeholder="Ngưỡng trên" ng-model="tosend.newmax">',
			title: name,
			subTitle: 'Nhập ngưỡng cảnh báo mới',
			scope: $scope,
			buttons: [{ text: 'Cancel' },{text: '<b>Save</b>',type: 'button-positive',
				onTap: function(e) {
					if ((!$scope.tosend.newmin)||(!$scope.tosend.newmax)) {
						e.preventDefault();
					} else {
						return $scope.tosend;
					}
				}
			},]
		});
		myPopup.then(function(res) {
			DB.ChangeRange(res.id,res.type,res.newmin,res.newmax).then(function(result) {
				console.log(res.type);
				$ionicPopup.alert({
					title: 'Kết quả',
					template: result.data.result
				});
				gettemp();
			},function(err) {console.log(err);}
			);
		});
	}
	$scope.hide=function(){
		console.log('d');
		$scope.showRename=false;
		$scope.showEdit=false;
	};
	gettemp=function(){
		DB.gettemp().then(function(result) {
			$scope.temps=result.data;
		},function(err) {
			console.log(err);
		});
	};
	gettemp();
})
.controller('setup_humi', function($scope,$ionicPopup,$timeout,DB) {
	$scope.humis={};
	$scope.showEdit=false;
	$scope.Edit=function(){
		$scope.showEdit=!$scope.showEdit;
		$scope.showRename=false;
	};
	$scope.visRename=function(){
		$scope.showRename=!$scope.showRename;
		$scope.showEdit=false;
	};
	$scope.rename=function(id,name){
		$scope.tosend={};
		$scope.tosend.id=id;
		var myPopup = $ionicPopup.show({
			template: '<input type="text" ng-model="tosend.newname">',
			title: name,
			subTitle: 'Nhập tên mới',
			scope: $scope,
			buttons: [{ text: 'Cancel' },{text: '<b>Save</b>',type: 'button-positive',
				onTap: function(e) {
					if (!$scope.tosend.newname) {
						e.preventDefault();
					} else {
						return $scope.tosend;
					}
				}
			},]
		});
		myPopup.then(function(res) {
			DB.RenameDevice(res.id,res.newname).then(function(result) {
				$ionicPopup.alert({
					title: 'Kết quả',
					template: result.data.result
				});
			},function(err) {console.log(err);}
			);
		});
	}
	$scope.changerange=function(id,name,oldmin,oldmax){
		$scope.tosend={};
		$scope.tosend.id=id;
		$scope.tosend.type='humi';
		var myPopup = $ionicPopup.show({
			template: '<input type="number" placeholder="Ngưỡng dưới" ng-model="tosend.newmin"><input type="number" placeholder="Ngưỡng trên" ng-model="tosend.newmax">',
			title: name,
			subTitle: 'Nhập ngưỡng cảnh báo mới',
			scope: $scope,
			buttons: [{ text: 'Cancel' },{text: '<b>Save</b>',type: 'button-positive',
				onTap: function(e) {
					if ((!$scope.tosend.newmin)||(!$scope.tosend.newmax)) {
						e.preventDefault();
					} else {
						return $scope.tosend;
					}
				}
			},]
		});
		myPopup.then(function(res) {
			DB.ChangeRange(res.id,res.type,res.newmin,res.newmax).then(function(result) {
				console.log(res.type);
				$ionicPopup.alert({
					title: 'Kết quả',
					template: result.data.result
				});
				gethumi();
			},function(err) {console.log(err);}
			);
		});
	}
	$scope.hide=function(){
		console.log('d');
		$scope.showRename=false;
		$scope.showEdit=false;
	};
	gethumi=function(){
		DB.gethumi().then(function(result) {
			$scope.humis=result.data;
		},function(err) {
			console.log(err);
		});
	};
	gethumi();
})
