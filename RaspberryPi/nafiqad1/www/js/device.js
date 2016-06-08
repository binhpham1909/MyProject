myapp
.controller('device', function($scope) {
})
.controller('device_add', function($scope,$ionicPopup,DB) {
	$scope.AddDevice=function(){
		DB.AddDevice($scope.serial,$scope.name).then(function(result) {
			$ionicPopup.alert({
				title: 'Kết quả',
				template: result.data.result
			});
		},function(err) {console.log(err);}
		);
	}
})
.controller('device_config', function($scope,$ionicPopup,$timeout,DB) {
	$scope.alerts = { checked: true };
	$scope.ring = { checked: true };
	$scope.sms = { checked: true };
	$scope.email = { checked: true };
	$scope.alertschange = function() {
		$timeout(function() {
			DB.SetConfig("alert",$scope.alerts.checked).then(function(result) {
				var mypop=$ionicPopup.alert({
				title: 'Kết quả',
				template: result.data.result
				});
				ReadConfig();
				$timeout(function() {mypop.close();}, 2000);
			},function(err) {console.log(err);});
			});
	}
	$scope.ringchange = function() {
		$timeout(function() {
			DB.SetConfig("ring",$scope.ring.checked).then(function(result) {
				var mypop=$ionicPopup.alert({
					title: 'Kết quả',
					template: result.data.result
				});
				ReadConfig();
				$timeout(function() {mypop.close();}, 2000);
			},function(err) {console.log(err);});
			});
	}
	$scope.smschange = function() {
		$timeout(function() {
			DB.SetConfig("sms",$scope.sms.checked).then(function(result) {
				var mypop=$ionicPopup.alert({
					title: 'Kết quả',
					template: result.data.result
				});
				ReadConfig();
				$timeout(function() {mypop.close();}, 2000);
			},function(err) {console.log(err);});
			});
	}
	$scope.emailchange = function() {
		$timeout(function() {
			DB.SetConfig("email",$scope.email.checked).then(function(result) {
				var mypop=$ionicPopup.alert({
					title: 'Kết quả',
					template: result.data.result
				});
				ReadConfig();
				$timeout(function() {mypop.close();}, 2000);
			},function(err) {console.log(err);});
		});
	}
	var ReadConfig=function(){
		DB.ReadConfig().then(function(result) {
			if(result.data.alert=='1') $scope.alerts.checked=true; else $scope.alerts.checked=false;
			if(result.data.ring=='1') $scope.ring.checked=true; else $scope.ring.checked=false;
			if(result.data.sms=='1') $scope.sms.checked=true; else $scope.sms.checked=false;
			if(result.data.email=='1') $scope.email.checked=true; else $scope.email.checked=false;
			},function(err) {console.log(err);}
		);		
	}
	ReadConfig();
})
.controller('device_sms', function($scope,$ionicPopup,$timeout,DB) {
	$scope.show={del:false,edit:false};
	$scope.users={};
	$scope.edit = function(id,name) {
		$scope.tosend={};
		$scope.tosend.id=id;
		$scope.tosend.content='sms';
		var myPopup = $ionicPopup.show({
			template: '<input type="text" placeholder="Tên" ng-model="tosend.newname"><input type="text" placeholder="SĐT" ng-model="tosend.newphone">',
			title: name,
			subTitle: 'Nhập thông tin mới',
			scope: $scope,
			buttons: [{ text: 'Cancel' },{text: '<b>Save</b>',type: 'button-positive',
				onTap: function(e) {
					if ((!$scope.tosend.newname)||(!$scope.tosend.newphone)) {
						e.preventDefault();
					} else {
						return $scope.tosend;
					}
				}
			},]
		});
		myPopup.then(function(res) {
			DB.EditUserReceive(res.content,res.id,res.newname,res.newphone).then(function(result) {
				var mypop=$ionicPopup.alert({
					title: 'Kết quả',
					template: result.data.result
				});
				Read();
				$timeout(function() {mypop.close();}, 2000);
			},function(err) {console.log(err);}
			);
		});
	}
	$scope.add = function() {
		$scope.tosend={};
		$scope.tosend.content='sms';
		var myPopup = $ionicPopup.show({
			template: '<input type="text" placeholder="Tên" ng-model="tosend.newname"><p></p><input type="text" placeholder="SĐT" ng-model="tosend.newphone">',
			title: 'Thêm mới',
			subTitle: 'Nhập thông tin mới',
			scope: $scope,
			buttons: [{ text: 'Cancel' },{text: '<b>Save</b>',type: 'button-positive',
				onTap: function(e) {
					if ((!$scope.tosend.newname)||(!$scope.tosend.newphone)) {
						e.preventDefault();
					} else {
						return $scope.tosend;
					}
				}
			},]
		});
		myPopup.then(function(res) {
			DB.AddUserReceive(res.content,res.newname,res.newphone).then(function(result) {
				var mypop=$ionicPopup.alert({
					title: 'Kết quả',
					template: result.data.result
				});
				Read();
				$timeout(function() {mypop.close();}, 2000);
			},function(err) {console.log(err);}
			);
		});
	}
	$scope.del = function(id,name,phone) {
		$scope.tosend={};
		$scope.tosend.content='sms';
		$scope.tosend.id=id;
		var myPopup = $ionicPopup.show({
			template: name+'<p>'+phone+'</p>',
			title: 'Xác nhận xóa',
			subTitle: name,
			scope: $scope,
			buttons: [{ text: 'Cancel' },{text: '<b>Xóa</b>',type: 'button-assertive',
				onTap: function(e) {
					if (!$scope.tosend.id) {
						e.preventDefault();
					} else {
						return $scope.tosend;
					}
				}
			},]
		});
		myPopup.then(function(res) {
			DB.DelUserReceive(res.content,res.id).then(function(result) {
				var mypop=$ionicPopup.alert({
					title: 'Kết quả',
					template: result.data.result
				});
				Read();
				$timeout(function() {mypop.close();}, 2000);
			},function(err) {console.log(err);}
			);				
		});
	};
	var Read=function(){
		DB.ReadUserReceive('sms').then(function(result) {
			$scope.users=result.data;
			},function(err) {console.log(err);}
		);		
	}
	Read();
})
.controller('device_email', function($scope,$ionicPopup,$timeout,DB) {
	$scope.show={del:false,edit:false};
	$scope.users={};
	$scope.edit = function(id,name) {
		$scope.tosend={};
		$scope.tosend.id=id;
		$scope.tosend.content='email';
		var myPopup = $ionicPopup.show({
			template: '<input type="text" placeholder="Tên" ng-model="tosend.newname"><input type="text" placeholder="SĐT" ng-model="tosend.newemail">',
			title: name,
			subTitle: 'Nhập thông tin mới',
			scope: $scope,
			buttons: [{ text: 'Cancel' },{text: '<b>Save</b>',type: 'button-positive',
				onTap: function(e) {
					if ((!$scope.tosend.newname)||(!$scope.tosend.newemail)) {
						e.preventDefault();
					} else {
						return $scope.tosend;
					}
				}
			},]
		});
		myPopup.then(function(res) {
			DB.EditUserReceive(res.content,res.id,res.newname,res.newemail).then(function(result) {
				var mypop=$ionicPopup.alert({
					title: 'Kết quả',
					template: result.data.result
				});
				Read();
				$timeout(function() {mypop.close();}, 2000);
			},function(err) {console.log(err);}
			);
		});
	}
	$scope.add = function() {
		$scope.tosend={};
		$scope.tosend.content='email';
		var myPopup = $ionicPopup.show({
			template: '<input type="text" placeholder="Tên" ng-model="tosend.newname"><p></p><input type="text" placeholder="Email" ng-model="tosend.newemail">',
			title: 'Thêm mới',
			subTitle: 'Nhập thông tin mới',
			scope: $scope,
			buttons: [{ text: 'Cancel' },{text: '<b>Save</b>',type: 'button-positive',
				onTap: function(e) {
					if ((!$scope.tosend.newname)||(!$scope.tosend.newemail)) {
						e.preventDefault();
					} else {
						return $scope.tosend;
					}
				}
			},]
		});
		myPopup.then(function(res) {
			DB.AddUserReceive(res.content,res.newname,res.newemail).then(function(result) {
				var mypop=$ionicPopup.alert({
					title: 'Kết quả',
					template: result.data.result
				});
				Read();
				$timeout(function() {mypop.close();}, 2000);
			},function(err) {console.log(err);}
			);
		});
	}
	$scope.del = function(id,name,phone) {
		$scope.tosend={};
		$scope.tosend.content='email';
		$scope.tosend.id=id;
		var myPopup = $ionicPopup.show({
			template: name+'<p>'+phone+'</p>',
			title: 'Xác nhận xóa',
			subTitle: name,
			scope: $scope,
			buttons: [{ text: 'Cancel' },{text: '<b>Xóa</b>',type: 'button-assertive',
				onTap: function(e) {
					if (!$scope.tosend.id) {
						e.preventDefault();
					} else {
						return $scope.tosend;
					}
				}
			},]
		});
		myPopup.then(function(res) {
			DB.DelUserReceive(res.content,res.id).then(function(result) {
				var mypop=$ionicPopup.alert({
					title: 'Kết quả',
					template: result.data.result
				});
				Read();
				$timeout(function() {mypop.close();}, 2000);
			},function(err) {console.log(err);}
			);				
		});
	};
	var Read=function(){
		DB.ReadUserReceive('email').then(function(result) {
			$scope.users=result.data;
			},function(err) {console.log(err);}
		);		
	}
	Read();
})