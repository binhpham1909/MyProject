angular.module('services', [])
.factory('CacheService', function($cacheFactory) {
    return $cacheFactory('CacheService');
})
.factory('CACHE', function(CacheService) {
    return {
        getVal: function(key) {
            var news = CacheService.get(key);
            if(news) {
                return news;
            }
            return null;
        },
        setVal: function(key, value) {
            CacheService.put(key, value);
        },
        clear: function(key) {
            CacheService.put(key, '');
        },
        remove: function(key) {
            CacheService.remove(key);
        },
        removeAll: function() {
            CacheService.removeAll();
        }
    };
})
.factory('LOGOUT', function($http) {
    return {
        logout: function() {
			return $http.post('process/login.php',{'action':'logout'});
        }
    };
})
.factory('DB', function($http) {
    return {
        gettemp: function() {
            return $http.post('process/p.status.sensor.php',{'sensor':'temp'});
        },
		gethumi: function() {
            return $http.post('process/p.status.sensor.php',{'sensor':'humi'});
        },
		RenameDevice: function(nameid,newname) {
            return $http.post('process/p.device.php',{'action':'rename','nameid':nameid,'newname':newname});
        },
		ChangeRange: function(id,type,min,max) {
            return $http.post('process/p.sensor.range.php',{'type':type,'id':id,'min':min,'max':max});
        },
		AddDevice: function(serial,name) {
            return $http.post('process/p.device.php',{'action':'add','serial':serial,'name':name});
        },
		SetConfig: function(key,value) {
            return $http.post('process/p.config.php',{'action':'edit','key':key,'value':value});
        },
		ReadConfig: function() {
            return $http.post('process/p.config.php',{'action':'read'});
        },
		ReadUserReceive: function(content) {
            return $http.post('process/p.user.php',{'action':'read','content':content});
        },
		EditUserReceive: function(content,id,newname,newvalue) {
            return $http.post('process/p.user.php',{'action':'edit','content':content,'id':id,'name':newname,'value':newvalue});
        },
		AddUserReceive: function(content,name,newvalue) {
            return $http.post('process/p.user.php',{'action':'add','content':content,'name':name,'value':newvalue});
        },
		DelUserReceive: function(content,id) {
            return $http.post('process/p.user.php',{'action':'del','content':content,'id':id});
        }
    };
})