var facebookSDK = {
	hasInitialized:false
	, isLogIn:false
	, accessToken:null
	// @ Private
	, loadSDK:function() {

	    if(this.hasInitialized) {
			return;
	    }

	    (function(d, s, id){
		        var js, fjs = d.getElementsByTagName(s)[0];
		        if (d.getElementById(id)) {return;}
		        js = d.createElement(s); js.id = id;
		        js.src = "//connect.facebook.net/en_US/sdk.js";
		        fjs.parentNode.insertBefore(js, fjs);
			}(document, 'script', 'facebook-jssdk')
	    );
	}
	, callback_arr:[]
	, add_event_callback:function(callback, scope) {

		if(callback == null) {
			return;
		}
		if(scope == null) {
			return;
		}

		var callback_set = {
			callback:callback
			, scope:scope
		};

		this.callback_arr.push(callback_set);

	}
	// @ Public
	, FBAppId:""
	, FBSDKVer:""
	, init_FB:function(appId, version, callback, callbackScope, paramObj) {

		if(this.hasInitialized) {
			console.log("!Error! / init / this.hasInitialized");
			return;
		}
		if(appId == null) {
			console.log("!Error! / init / appId == null");
			return;
		}
		if(version == null) {
			console.log("!Error! / init / version == null");
			return;
		}
		if(callback == null) {
			console.log("!Error! / logIn / callback == null");
			return;
		}
		if(callbackScope == null) {
			console.log("!Error! / logIn / callbackScope == null");
			return;
		}

		this.FBAppId = appId;
		this.FBSDKVer = version;

		var _self = this;
		window.fbAsyncInit = function() {

			FB.init({
				appId      : appId,
				xfbml      : true,
				version    : version
			});

			_self.hasInitialized = true;

			// it calls when SDK is ready.
			if(callback != null && callbackScope != null) {
				callback.apply(callbackScope, [paramObj]);	
			}
			
		};

		// Load the SDK asynchronously
		this.loadSDK();
    
	}

	, FACEBOOK_SDK_PRODUCT_APP_DOMAIN:""
	, FACEBOOK_SDK_PRODUCT_APP_ID:""
	, FACEBOOK_SDK_PRODUCT_VERSION:"v2.6"

	, FACEBOOK_SDK_STAGE_APP_DOMAIN:""
	, FACEBOOK_SDK_STAGE_APP_ID:"619046961592129"
	, FACEBOOK_SDK_STAGE_VERSION:"v2.6"

	, FACEBOOK_SDK_DEV_APP_DOMAIN:"dev.xogames.co.kr"
	, FACEBOOK_SDK_DEV_APP_ID:"1583644885261297"
	, FACEBOOK_SDK_DEV_VERSION:"v2.6"

	, FACEBOOK_SDK_LOCAL_APP_DOMAIN:"wonder.xogames.co.kr" // "52.78.18.208"
	, FACEBOOK_SDK_LOCAL_APP_ID:"255436801488680"
	, FACEBOOK_SDK_LOCAL_VERSION:"v2.6"

	, init:function(callback, callback_scope) {

		var root_domain = _server.get_root_domain();

		var fb_app_id = "";
		var fb_app_ver = "";

		var _self = this;

		// NOT IMPLEMENTED YET
/*
		if(-1 < root_domain.indexOf(this.FACEBOOK_SDK_PRODUCT_APP_DOMAIN)) {

			fb_app_id = this.FACEBOOK_SDK_PRODUCT_APP_ID;
			fb_app_ver = this.FACEBOOK_SDK_PRODUCT_VERSION;

			facebookSDK.init_FB(fb_app_id, fb_app_ver, function(paramObj){

				if(callback != null && callback_scope != null) {
					callback.apply(callback_scope,[]);	
				}

				var callback_arr = _self.callback_arr;
				for(var idx=0;idx < callback_arr.length;idx++) {
					var callback_set = callback_arr[idx];
					callback_set.callback.apply(callback_set.scope,[]);
				}

			}, this);

		} else if(-1 < root_domain.indexOf(this.FACEBOOK_SDK_STAGE_APP_DOMAIN)) {

			fb_app_id = this.FACEBOOK_SDK_STAGE_APP_ID;
			fb_app_ver = this.FACEBOOK_SDK_STAGE_VERSION;

			facebookSDK.init_FB(fb_app_id, fb_app_ver, function(paramObj){

				if(callback != null && callback_scope != null) {
					callback.apply(callback_scope,[]);	
				}

				var callback_arr = _self.callback_arr;
				for(var idx=0;idx < callback_arr.length;idx++) {
					var callback_set = callback_arr[idx];
					callback_set.callback.apply(callback_set.scope,[]);
				}

			}, this);

		} else 
*/		
		if(-1 < root_domain.indexOf(this.FACEBOOK_SDK_DEV_APP_DOMAIN)) {

			fb_app_id = this.FACEBOOK_SDK_DEV_APP_ID;
			fb_app_ver = this.FACEBOOK_SDK_DEV_VERSION;

			facebookSDK.init_FB(fb_app_id, fb_app_ver, function(paramObj){

				if(callback != null && callback_scope != null) {
					callback.apply(callback_scope,[]);	
				}

				var callback_arr = _self.callback_arr;
				for(var idx=0;idx < callback_arr.length;idx++) {
					var callback_set = callback_arr[idx];
					callback_set.callback.apply(callback_set.scope,[]);
				}
				
			}, this);

		} else if(-1 < root_domain.indexOf(this.FACEBOOK_SDK_LOCAL_APP_DOMAIN)) {

			fb_app_id = this.FACEBOOK_SDK_LOCAL_APP_ID;
			fb_app_ver = this.FACEBOOK_SDK_LOCAL_VERSION;

			facebookSDK.init_FB(fb_app_id, fb_app_ver, function(paramObj){

				if(callback != null && callback_scope != null) {
					callback.apply(callback_scope,[]);	
				}

				var callback_arr = _self.callback_arr;
				for(var idx=0;idx < callback_arr.length;idx++) {
					var callback_set = callback_arr[idx];
					callback_set.callback.apply(callback_set.scope,[]);
				}
				
			}, this);			

		} else {

			console.log("Please check root domain. It is not the FB app.");

		}

	}
	// @ Private
	// @ Desc : log in by user FB account.
	, FB_PERMISSION_PAGE_MANAGER:{scope: 'email,publish_actions,publish_pages,manage_pages,read_insights'}	
	, FB_PERMISSION_MAGENDAS_USER:{scope: 'email'}
	, logIn:function(callback, callbackScope, paramObj) { // facebookSDK.logIn()

		if(callback == null) {
			console.log("!Error! / logIn / callback == null");
			return;
		}
		if(callbackScope == null) {
			console.log("!Error! / logIn / callbackScope == null");
			return;
		}

		var permisson = this.FB_PERMISSION_PAGE_MANAGER;
		if(paramObj != null && paramObj.permisson != null) {
			permisson = paramObj.permisson;
		}
		var nextParamObj = {
			callback:callback
			,callbackScope:callbackScope
			,paramObj:paramObj
		}

		if(!this.isLogIn) {

			var callback = function(paramObj) {

				console.log("logIn / paramObj ::: ",paramObj);

				//isLogIn
				if(paramObj != null && paramObj.isLogIn) {

					paramObj.callback.apply(paramObj.callbackScope,[paramObj.paramObj]);
				  
				} else {

					FB.login(function(response){

						// Handle the response object, like in statusChangeCallback() in our demo code.
						// paramObj
						var callback = paramObj.callback;
						var callbackScope = paramObj.callbackScope;
						var nextParamObj = paramObj.paramObj;

						nextParamObj.response = response;
						callback.apply(callbackScope, [nextParamObj]);

					}, permisson);

				}

			} // end callback
			var callbackScope = this;

			this.checkLoginState(callback, callbackScope, nextParamObj);

		} else {

			FB.login(function(response){

				// Handle the response object, like in statusChangeCallback() in our demo code.
				console.log("logIn / response :: ",response);

				paramObj.response = response;
				callback.apply(callbackScope, [paramObj]);

			}, {scope: 'email,publish_actions,publish_pages,manage_pages,read_insights'});

		}

	}
	, logOut:function() { // facebookSDK.logOut();

		if(!this.hasInitialized) {
			console.log("!Error! / logIn / !this.hasInitialized");
			return;
		}
		if(!this.isLogIn) {
			console.log("!Error! / logIn / !this.isLogIn");
			return; 
		}

		FB.logout(function(response){
			// Handle the response object, like in statusChangeCallback() in our demo code.
			console.log("logIn / response :: ",response);
		});

	}

	// @ Private
	, checkLoginState:function(callback, callbackScope, paramObj) {

		if(!this.hasInitialized) {
		  console.log("!Error! / initLogIn / !this.hasInitialized");
		  return;
		}
		if(callback == null) {
		  console.log("!Error! / initLogIn / callback == null");
		  return;
		}
		if(callbackScope == null) {
		  console.log("!Error! / initLogIn / callbackScope == null");
		  return;
		}
		if(paramObj == null) {
		  paramObj = {};
		}

		// This is called with the results from from FB.getLoginStatus().
		var _self = this;
		FB.getLoginStatus(function(response) {

			_self.isLogIn = true;
			var isLogIn = false;
			if (response.status === 'connected') {
				// Logged into your app and Facebook.
				isLogIn = true;
			} else if (response.status === 'not_authorized') {
				// The person is logged into Facebook, but not your app.
				isLogIn = false;
			} else {
				// The person is not logged into Facebook, so we're not sure if they are logged into this app or not.
				isLogIn = false;
			}
			paramObj.isLogIn = isLogIn;

			//callback, callbackScope        
			callback.apply(callbackScope,[paramObj]);

		});

	}
	// @ Public
	, getMe:function(callback, callbackScope, paramObj) {

		// wonder.jung
		if(callback == null) {
		  console.log("!Error! / getMe / callback == null");
		  return;
		}
		if(callbackScope == null) {
		  console.log("!Error! / getMe / callbackScope == null");
		  return;
		}
		var fields = {"fields":"id,name,picture,email"};
		if(paramObj.fields != null) {
			fields = paramObj.fields;
		}

		FB.api(
			'/me',
			'GET',
			fields,
			function(response) {
				// Insert your code here
				callback.apply(callbackScope, [response]);
			}
		);    

	}
	// @ Public
	, getPermission:function() {

		if(!this.isLogIn) {
			console.log("!Error! / getMe / !this.isLogIn");
			return; 
		}

		FB.api('/me/permissions', function(response) {
			console.log("HERE / 001 / response :: ",response);
		});

	}


	// @ Private
	, drawPagePost:function(callback, callbackScope, paramObj) {

		if(paramObj == null) {
			console.log("!Error! / drawPagePost / paramObj == null");
			return;
		}
		var pageId = paramObj.pageId;
		if(_v.is_not_valid_str(pageId)) {
			console.log("!Error! / drawPagePost / _v.is_not_valid_str(pageId)");
			return;
		}
		var parentJq = paramObj.parentJq;
		if(parentJq == null) {
			console.log("!Error! / drawPagePost / parentJq == null");
			return;
		}

		var nextParamObj = {
			callback:callback,
			callbackScope:callbackScope,
			paramObj:paramObj,
			pageId:pageId
		}


		// 1. fetch page post list
		var _self = this;
		var callbackPagePost = function(paramObj) {

			var postList = paramObj.postList;

			_self.setPostCotentInfo(callbackContent, callbackContentScope, paramObj);

		}
		var callbackScopePagePost = this;
		this.getPagePosts(callbackPagePost, callbackScopePagePost, nextParamObj);

		// 2. set post contetn info
		var callbackContent = function(paramObj) {

			_self.setPostViewCnt(callbackViewCnt, callbackViewCntScope, paramObj);

		}
		var callbackContentScope = this;

		// 3. set post view info
		var callbackViewCnt = function(paramObj) {

			var postList = paramObj.postList;
			_self.setPagePicture(callbackDrawList, callbackDrawListScope, paramObj);
		}
		var callbackViewCntScope = this;

		// 4. get page info
		var callbackDrawList = function(paramObj) {

			var pagePictureUrl = paramObj.pagePictureUrl;
			var postList = paramObj.postList;

			var tag = "<div class=\"well\" style=\"width:500px;padding-bottom:0px;padding-left:7px;padding-top:7px;\">";

			for(var idx=0; idx < postList.length; idx++) {
				var postObj = postList[idx];

				var message = "";
				if(postObj.message != null && postObj.message != "") {
					message = postObj.message;
				}

				var pageName = "";
				if(	postObj.from != null && 
					postObj.from.name != null && 
					postObj.from.name != "") {

					pageName = postObj.from.name;
				}

				var createdTime = postObj.created_time.replace(/T/gi, " ").replace(/\+\d+/gi, "");
				var viewCntMsg = createdTime;
				var viewCnt = postObj.viewCnt;
				if(0 < viewCnt) {
					viewCntMsg += " " + viewCnt + " viewed";
				}

				tag += ""
				+ "<div class=\"row\">"
					+ "<div class=\"col-sm-6 col-md-4\">"
				;

				// page thumbnail
				tag += ""
						+ "<a href=\"#\" class=\"thumbnail\" style=\"float:left;margin:5px;width:54px;\">"
							+ "<img src=\"{src}\" alt=\"...\">".replace(/\{src\}/gi, pagePictureUrl)
						+ "</a>"
						+ "<div class=\"thumbnail\" style=\"width:484px;margin-bottom:7px;\">"
				;

				if(postObj.photo != null) {

					var source = postObj.photo.images[2].source;

					tag += ""
							+ "<div class=\"caption\">"
								+ "<h5 style=\"margin-bottom:32px;\">{title}<span style=\"font-size:14px;color:#ccc;margin-left:20px;\">{view_cnt}</span></h5>"
								.replace(/\{title\}/gi, pageName)
								.replace(/\{view_cnt\}/gi, viewCntMsg)
							+ "</div>"

							+ "<img src=\"{src}\" alt=\"...\">".replace(/\{src\}/gi, source)

							+ "<div class=\"caption\">"
								+ "<p style=\"margin-bottom:0px;\">{message}</p>".replace(/\{message\}/gi, message)
								// + "<p><a href=\"#\" class=\"btn btn-primary\" role=\"button\">Button</a> <a href=\"#\" class=\"btn btn-default\" role=\"button\">Button</a></p>"
							+ "</div>"
					// + "</div>"
					;

				} else if(postObj.video != null) {

					var embedHtml = postObj.video.embed_html;

					tag += ""
					// + "<div class=\"thumbnail\" style=\"width:484px;margin-bottom:7px;\">"
							+ "<div class=\"caption\">"
								+ "<h5 style=\"margin-bottom:32px;\">{title}<span style=\"font-size:14px;color:#ccc;margin-left:20px;\">{view_cnt}</span></h5>"
								.replace(/\{title\}/gi, pageName)
								.replace(/\{view_cnt\}/gi, viewCntMsg)
							+ "</div>"

							+ embedHtml.replace(/400/gi, 450)

							+ "<div class=\"caption\">"
								+ "<p style=\"margin-bottom:0px;\">{message}</p>".replace(/\{message\}/gi, message)
							+ "</div>"
					// + "</div>"
					;


				} else {

					tag += ""
					// + "<div class=\"thumbnail\" style=\"width:484px;margin-bottom:7px;\">"
							+ "<div class=\"caption\">"
								+ "<h5 style=\"margin-bottom:32px;\">{title}<span style=\"font-size:14px;color:#ccc;margin-left:20px;\">{view_cnt}</span></h5>"
								.replace(/\{title\}/gi, pageName)
								.replace(/\{view_cnt\}/gi, viewCntMsg)
								+ "<p style=\"margin-bottom:0px;\">{message}</p>".replace(/\{message\}/gi, message)
							+ "</div>"
					
					;
				}

				// DEBUG - INIT
				tag += ""	
							+ "<ul class=\"list-group\" style=\"margin-bottom:0px;\">"
								+ "<li class=\"list-group-item\" style=\"padding:3px;font-size:10px;color:#5B74B3;\"><strong>{key} : </strong>{value}</li>"
									.replace(/\{key\}/gi, "id")
									.replace(/\{value\}/gi, postObj.id)
								+ "<li class=\"list-group-item\" style=\"padding:3px;font-size:10px;color:#5B74B3;\"><strong>{key} : </strong>{value}</li>"
									.replace(/\{key\}/gi, "is_published")
									.replace(/\{value\}/gi, postObj.is_published)
								+ "<li class=\"list-group-item\" style=\"padding:3px;font-size:10px;color:#5B74B3;\"><strong>{key} : </strong>{value}</li>"
									.replace(/\{key\}/gi, "is_hidden")
									.replace(/\{value\}/gi, postObj.is_hidden)
								+ "<li class=\"list-group-item\" style=\"padding:3px;font-size:10px;color:#5B74B3;\"><strong>{key} : </strong>{value}</li>"
									.replace(/\{key\}/gi, "type")
									.replace(/\{value\}/gi, postObj.type)
								+ "<li class=\"list-group-item\" style=\"padding:3px;font-size:10px;color:#5B74B3;\"><strong>{key} : </strong>{value}</li>"
									.replace(/\{key\}/gi, "status_type")
									.replace(/\{value\}/gi, postObj.status_type)

				if(postObj.link != null && postObj.link != "") {
					tag += ""
								+ "<li class=\"list-group-item\" style=\"padding:3px;font-size:10px;color:#5B74B3;\"><a href=\"{value}\"><strong>{key}</strong></a></li>"
									.replace(/\{key\}/gi, "link")
									.replace(/\{value\}/gi, postObj.link)
					;
				}
				// DEBUG - END

				tag += ""	
							+ "</ul>"
						+ "</div>"
					+ "</div>"
				+ "</div>"
				;
			}
			tag += "</div>"
			; 

			parentJq.append(tag);

		}
		var callbackDrawListScope = this;
	}

	// @ Private
	, setPagePicture:function(callback, callbackScope, paramObj) {

		if(callback == null) {
			console.log("!Error! / getPageInfo / callback == null");
			return;
		}
		if(callbackScope == null) {
			console.log("!Error! / getPageInfo / callbackScope == null");
			return;
		}
		if(paramObj == null) {
			console.log("!Error! / getPageInfo / paramObj == null");
			return;
		}
		var pageId = paramObj.pageId;
		if(pageId == null) {
			console.log("!Error! / getPageInfo / paramObj == null");
			return;
		}

		// No log in needed.

		FB.api(
			'/{page-id}/picture'.replace(/\{page\-id\}/gi, pageId),
			'GET',
			{},
			function(response) {

				console.log("getPagePicture / response ::: ",response);
				
				var pagePictureUrl = "";
				if(response.data != null && response.data.url != null) {

					pagePictureUrl = response.data.url;	
					paramObj.pagePictureUrl = pagePictureUrl;

					callback.apply(callbackScope,[paramObj]);

				} // end if
			} // end callback
		); // end api

	}
	// @ Private
	, setPostViewCnt:function(callback, callbackScope, paramObj) {

		// param - postList
		if(paramObj == null) {
			console.log("!Error! / setPostViewCnt / paramObj == null");
			return;
		}

		var nextParamObj = {
			callback:callback
			, callbackScope:callbackScope
			, paramObj:paramObj
		}

		var _self = this;
		var callbackViewCnt = function(paramObj) {

			console.log("XXX / paramObj ::: ",paramObj);

			var postList = paramObj.paramObj.postList;
			if(postList == null || 0 == postList.length) {
				console.log("!Error! / setPostViewCnt / postList is not valid!");
				return;
			}

			// Multiple Request - init.
			var _url = "https://graph.facebook.com";
			var access_token = paramObj.accessToken;
			var postListForBatch = paramObj.paramObj.postList;

			var batchReqJSONArr = [];
			for(var idx = 0; idx < postListForBatch.length; idx++) {
				var postObj = postListForBatch[idx];
				var postId = postObj.id;

				// TODO - wonder.jung
				if(postObj.video != null) {

					// video view - total_video_views_unique
					// https://developers.facebook.com/docs/graph-api/reference/video/video_insights/

					//233311017036635_235545043479899 / /233311017036635_235545043479899/insights/post_video_views/lifetime
					
					var reqObj = {
						"method":"GET"
						, "relative_url":"/{post-id}/insights/post_video_views/lifetime".replace(/\{post\-id\}/gi, postId)
					};
					batchReqJSONArr.push(reqObj);

				} else {

					// photo view - 사진은 조회수가 없음.

					// -F ‘batch=[{“method”:”GET”, “relative_url”:”me”},{“method”:”GET”, “relative_url”:”me/friends?limit=50”}]’ \
					var reqObj = {
						"method":"GET"
						, "relative_url":"/{post-id}/insights/post_impressions/lifetime".replace(/\{post\-id\}/gi, postId)
					};
					batchReqJSONArr.push(reqObj);

				}

			}
			var batchReqJSONArrStr = JSON.stringify(batchReqJSONArr);

			_ajax.post(
				// _url
				"https://graph.facebook.com"
				// _param_obj
				,{
					access_token:access_token
					,batch:batchReqJSONArrStr
				}
				// _delegate_after_job_done
				,_obj.get_delegate(
					// delegate_func
					function(data){

						var viewCntSet = {};
						if(data != null && 0 < data.length) {
							for(var idx=0; idx < data.length;idx++) {
								var resultObj = data[idx];
								var resultCode = resultObj.code;

								if(resultCode != 200) {
									console.log("resultObj ::: ",resultObj);
									console.log("!Error! / setPostViewCnt / resultCode != 200");
									continue;
								}
								var resultJSONStr = resultObj.body;
								var resultJSONObj = $.parseJSON(resultJSONStr);
								var resultObj = resultJSONObj.data[0];

								var name = resultObj.name;
								var viewCnt = resultObj.values[0].value;
								var postIdRaw = resultObj.id;
								var postId = postIdRaw.split("/")[0];

								viewCntSet[postId] = viewCnt;
							} // end for
						} // end if

						for(var idx=0; idx < postListForBatch.length;idx++) {
							var postObj = postListForBatch[idx];
							postObj.viewCnt = viewCntSet[postObj.id];
						}

						paramObj.callback.apply(paramObj.callbackScope, [paramObj.paramObj]);

					},
					// delegate_scope
					this
				)
			); // ajax done.
			// Multiple Request - done.

		}
		var callbackViewCntScope = this;

		this.getPageAccessTokenAsync(callbackViewCnt, callbackViewCntScope, nextParamObj);

	}	

	// @ Private
	, setPostCotentInfo:function(callback, callbackScope, paramObj) {

		// param - postList
		if(paramObj == null) {
			console.log("!Error! / setPostCotentInfo / paramObj == null");
			return;
		}
		var postList = paramObj.postList;
		if(postList == null || 0 == postList.length) {
			console.log("!Error! / setPostCotentInfo / postList is not valid!");
			return;
		}

		var _self = this;

		// Prepare multiple request query
		var callbackContentInfo = function(paramObj) {

			var access_token = paramObj.accessToken;
			var batchReqJSONArr = [];
			var postListForContentInfo = paramObj.postList;
			for(var idx = 0; idx < postListForContentInfo.length; idx++) {
				var postObj = postListForContentInfo[idx];
				var postId = postObj.id;
				var objectType = postObj.type;

				if("photo" === objectType) {

					var objectId = postObj.object_id;
					var reqObj = {
						"method":"GET"
						, "relative_url":"/{photo-id}?fields=id,album,from,height,width,images,link,picture".replace(/\{photo\-id\}/gi, objectId)
					};
					batchReqJSONArr.push(reqObj);

				} else if("video" === objectType) {

					var objectId = postObj.object_id;
					var reqObj = {
						"method":"GET"
						, "relative_url":"/{video-id}?fields=id,description,updated_time,embed_html,embeddable,from,length,picture,source,published".replace(/\{video\-id\}/gi, objectId),
					};
					batchReqJSONArr.push(reqObj);

				}		

			}
			var batchReqJSONArrStr = JSON.stringify(batchReqJSONArr);

			_ajax.post(
				// _url
				"https://graph.facebook.com"
				// _param_obj
				,{
					access_token:access_token
					,batch:batchReqJSONArrStr
				}
				// _delegate_after_job_done
				,_obj.get_delegate(
					// delegate_func
					function(data){

						var mediaSet = {};
						if(data != null && 0 < data.length) {


							for(var idx=0; idx < data.length;idx++) {
								var resultObj = data[idx];
								var resultCode = resultObj.code;

								if(resultCode != 200) {
									console.log("XXX / resultObj ::: ",resultObj);
									console.log("!Error! / setPostViewCnt / resultCode != 200");
									continue;
								}
								var resultJSONStr = resultObj.body;
								var resultJSONObj = $.parseJSON(resultJSONStr);
								mediaSet[resultJSONObj.id] = resultJSONObj;

							} // end for


						} // end if

						for(var idx=0; idx < postListForContentInfo.length;idx++) {

							var postObj = postListForContentInfo[idx];
							var postId = postObj.id;
							var objectType = postObj.type;
							var objectId = postObj.object_id;

							if("photo" === objectType) {

								postObj.photo = mediaSet[objectId];

							} else if("video" === objectType) {

								postObj.video = mediaSet[objectId];

							} // end if

						} // end for

						callback.apply(callbackScope, [paramObj]);

					},
					// delegate_scope
					this
				)
			); // ajax done.
			// Multiple Request - done.

		}
		var callbackContentInfoScope = this;

		this.getPageAccessTokenAsync(callbackContentInfo, callbackContentInfoScope, paramObj);

	}

	// @ Public
	, writePagePost:function(callback, callbackScope, paramObj) {

		if(callback == null) {
			console.log("!Error! / writePagePost / callback == null");
			return;
		}
		if(callbackScope == null) {
			console.log("!Error! / writePagePost / callbackScope == null");
			return;
		}
		if(paramObj == null) {
			console.log("!Error! / writePagePost / paramObj == null");
			return;
		}



		var _self = this;
		var callbackWritePagePost = function(paramObj) {

			console.log("callbackWritePagePost / paramObj ::: ",paramObj);

			if(paramObj == null) {
			console.log("!Error! / getPagePosts / paramObj == null");
			return;
			}

			var pageId = paramObj.pageId;
			if(_v.is_not_valid_str(pageId)) {
				console.log("!Error! / writePagePost / _v.is_not_valid_str(pageId)");
				return;
			}
			var message = paramObj.message;
			if(_v.is_not_valid_str(message)) {
				console.log("!Error! / writePagePost / _v.is_not_valid_str(message)");
				return;
			}
			var published = paramObj.published;
			if(published == null) {
				console.log("!Error! / writePagePost / published == null");
				return;
			}
			var scheduled_publish_time = paramObj.scheduled_publish_time;
			if(published == null) {
				console.log("!Error! / writePagePost / scheduled_publish_time == null");
				return;
			}
			var accessToken = paramObj.accessToken;
			if(accessToken == null) {
				console.log("!Error! / writePagePost / accessToken == null");
				return;
			}

			var fieldsObj = null;
			if(published) {
				fieldsObj = {"message":message,"published":published,"access_token":accessToken};
			} else {
				fieldsObj = {"message":message,"published":published,"scheduled_publish_time":scheduled_publish_time,"access_token":accessToken};
			}

			// wonder.jung
			FB.api(
			    "/{page-id}/feed".replace(/\{page\-id\}/gi, pageId),
			    "POST",
			    fieldsObj,        
			    function (response) {

					console.log("writePagePost / response ::: ",response);

					if (response && !response.error) {

						callback.apply(callbackScope,[paramObj]);

					}
			    }
			); // end api   

		}
		var callbackWritePagePostScope = this;

		if(!this.isLogIn) {

			// if not logged in, log in.
			this.logIn(function(paramObj) {

				this.getPageAccessTokenAsync(callbackWritePagePost, callbackWritePagePostScope, paramObj);

			}, this, nextParamObj);

		} else {

			this.getPageAccessTokenAsync(callbackWritePagePost, callbackWritePagePostScope, paramObj);

		}		
	}

	// @ Public
	, getPagePosts:function(callback, callbackScope, paramObj) {

		if(paramObj == null) {
			console.log("!Error! / getPagePosts / paramObj == null");
			return;
		}
		var pageId = paramObj.pageId;
		if(_v.is_not_valid_str(pageId)) {
			console.log("!Error! / getPagePosts / _v.is_not_valid_str(pageId)");
			return;
		}

		if(callback == null) {
			console.log("!Error! / getPagePosts / callback == null");
			return;
		}

		if(callbackScope == null) {
			console.log("!Error! / getPagePosts / callbackScope == null");
			return;
		}
		var nextParamObj = {
			callback:callback
			, callbackScope:callbackScope
			, paramObj:paramObj
			, pageId:pageId
		}

		var _self = this;
		var callback = function(paramObj) {

			if(paramObj == null) {
			console.log("!Error! / getPagePosts / paramObj == null");
			return;
			}

			console.log("HERE / 001 / paramObj ::: ",paramObj);

			// write the page post after getting Page Access Token
			FB.api(
				"/{page-id}/promotable_posts".replace(/\{page\-id\}/gi, paramObj.pageId),
				"GET",
				{"fields":"id,created_time,shares,type,status_type,properties,object_id,message,is_published,from,icon,link,is_hidden","include_hidden":"true"},
				function (response) {

					console.log("promotable_posts / response ::: ",response);

					if (response && !response.error) {
						/* handle the result */

						var postList = response.data;
						paramObj.paramObj.postList = postList;

						// wonder.jung
						paramObj.callback.apply(paramObj.callbackScope,[paramObj.paramObj]);

					} else {
						// TODO
					  	/* handle the error */

					}

				} // End callback

			); // End FB.api      

		}
		var callbackScope = this;

		if(!this.isLogIn) {

			// if not logged in, log in.
			this.logIn(function(paramObj) {

				console.log("HERE / 001 / paramObj ::: ",paramObj);

				this.getPageAccessTokenAsync(callback, callbackScope, paramObj);

			}, this, nextParamObj);

		} else {

			console.log("HERE / 002 / nextParamObj ::: ",nextParamObj);

			this.getPageAccessTokenAsync(callback, callbackScope, nextParamObj);

		}

	}
	// @ Private
	, pageAccessToken:{
		pageId:""
		,accessToken:""
	}
	, getPageAccessTokenAsync:function(callback, callbackScope, paramObj, forceToUpdate) {

		if(callback == null) {
			console.log("!Error! / getPageAccessTokenAsync / callback == null");
			return;
		}
		if(callbackScope == null) {
			console.log("!Error! / getPageAccessTokenAsync / callbackScope == null");
			return;
		}

		if(paramObj == null) {
			console.log("!Error! / getPageAccessTokenAsync / paramObj == null");
			return;
		}
		if(paramObj.pageId == null && this.pageAccessToken.pageId != "") {
			paramObj.accessToken = this.pageAccessToken.accessToken;
			callback.apply(callbackScope, [paramObj]);
			return;
		}
		var pageId = paramObj.pageId;
		if(_v.is_not_valid_str(pageId)) {
			console.log("!Error! / getPageAccessTokenAsync / _v.is_not_valid_str(pageId)");
			return;
		}
		if(forceToUpdate == null) {
			forceToUpdate = false;
		}
		if(this.pageAccessToken.pageId == -1 || (this.pageAccessToken.pageId != -1 && this.pageAccessToken.pageId != pageId)) {

			this.pageAccessToken.pageId=pageId;
			forceToUpdate = true;

		} else if(!forceToUpdate && this.pageAccessToken.pageId == pageId) {

			paramObj.accessToken = this.pageAccessToken.accessToken;
			callback.apply(callbackScope, [paramObj]);

			return;
		}

		var _self = this;
		var nextCallback = function(paramObj) {

			var callback = paramObj.callback;
			var callbackScope = paramObj.callbackScope;
			var paramObj = paramObj.paramObj;
			var pageId = paramObj.pageId;

			FB.api(
			"/{page-id}".replace(/\{page\-id\}/gi, pageId),
			'GET',
			{
			    "fields":"access_token"
			},

			function(response) {

			    if(response != null && response.access_token != null) {
			      if(forceToUpdate) {
			        _self.pageAccessToken.accessToken = response.access_token;
			      }
			      paramObj.accessToken = response.access_token;
			      callback.apply(callbackScope, [paramObj]);

			    }

			} // end callback

			); // end FB api

		}
		var nextCallbackScope = this;
		var nextParamObj = {
			callback:callback,
			callbackScope:callbackScope,
			paramObj:paramObj,
			pageId:pageId
		};
		this.logIn(nextCallback, nextCallbackScope, nextParamObj);

	}

	// wonder.jung

	// @ Public
	, PUBLISHED_CONTENT_TYPE_PUBLISHED:"PUBLISHED"
	, UNPUBLISHED_CONTENT_TYPE_SCHEDULED:"SCHEDULED"
	, UNPUBLISHED_CONTENT_TYPE_DRAFT:"DRAFT"
	, UNPUBLISHED_CONTENT_TYPE_ADS_POST:"ADS_POST"
	, isValidUnpublishedContentType:function(contentType) {

		if(contentType == null) {
			return false;
		}
		if( contentType === this.UNPUBLISHED_CONTENT_TYPE_SCHEDULED || 
		    contentType === this.UNPUBLISHED_CONTENT_TYPE_DRAFT || 
		    contentType === this.UNPUBLISHED_CONTENT_TYPE_ADS_POST) {

		    return true;
		}

		return false;
	}
	// @ Private
	,fileUpload:function(callback, callbackScope, paramObj) {

		var formUpload = paramObj.formUpload;
		if(formUpload == null) {
			console.log("!Error! / fileUpload / formUpload == null");
			return;
		}
		var url = paramObj.url;
		if(url == null) {
			console.log("!Error! / fileUpload / url == null");
			return;
		}
		var postParam = paramObj.postParam;


		// http://stackoverflow.com/questions/4264599/facebook-new-javascript-sdk-uploading-photos-with-it
		// Create an iframe 
		var iframe = document.createElement("iframe");
		iframe.setAttribute('id', "upload_iframe");
		iframe.setAttribute('name', "upload_iframe");
		iframe.setAttribute('width', "0px");
		iframe.setAttribute('height', "0px");
		iframe.setAttribute('border', "0");
		iframe.setAttribute('style', "width: 0; height: 0; border: none;");

		// Add to document.
		formUpload.parentNode.appendChild(iframe);
		window.frames['upload_iframe'].name = "upload_iframe";

		iframeId = document.getElementById("upload_iframe");

		// Add event to detect when upload has finished
		var eventHandler = function () {

			console.log("iframeId :: ",iframeId);

			if (iframeId.detachEvent){

				iframeId.detachEvent("onload", eventHandler);

			} else {

				iframeId.removeEventListener("load", eventHandler, false);

			}

			setTimeout(function() {
			  try {
					$('#upload_iframe').remove();
			  } catch (e) {
					console.log("e :: ",e);
			  }
			}, 100);

			// Uploading accomplised.
			callback.apply(callbackScope, [paramObj]);
		}

		if (iframeId.addEventListener) {
			iframeId.addEventListener('load', eventHandler, true);	
		}
		if (iframeId.attachEvent) {
			iframeId.attachEvent('onload', eventHandler);	
		}

		// Set properties of form...
		formUpload.setAttribute('target', 'upload_iframe');
		formUpload.setAttribute('action', url);
		formUpload.setAttribute('method', 'post');
		formUpload.setAttribute('enctype', 'multipart/form-data');
		formUpload.setAttribute('encoding', 'multipart/form-data');

		var formUploadJq = $(formUpload);

		if(postParam != null) {

			for(var key in postParam) {

				var value = postParam[key];
				var paramInput = 
				"<input type=\"hidden\" name=\"{name}\" value=\"{value}\"/>"
				.replace(/\{name\}/gi, key)
				.replace(/\{value\}/gi, value)
				;

				console.log("paramInput ::: ",paramInput);

				formUploadJq.append(paramInput);

			} // end for

		} // end if

		// Submit the form...
		formUpload.submit();

	}
	, uploadVideo:function(callback, callbackScope, paramObj) {

		if(paramObj == null) {
			console.log("!Error! / uploadVideo / paramObj == null");
			return;
		}

		var pageId = paramObj.pageId;
		if(_v.is_not_valid_str(pageId)) {
			console.log("!Error! / uploadVideo / _v.is_not_valid_str(pageId)");
			return;
		}

		paramObj.url = 
		"https://graph-video.facebook.com/{version}/{page-id}/videos"
		.replace(/\{version\}/gi, this.FBSDKVer)
		.replace(/\{page\-id\}/gi, pageId)
		;

		this.uploadMediaFile(callback, callbackScope, paramObj);
	}
	, uploadPhoto:function(callback, callbackScope, paramObj) {

		if(paramObj == null) {
			console.log("!Error! / uploadVideo / paramObj == null");
			return;
		}

		var pageId = paramObj.pageId;
		if(_v.is_not_valid_str(pageId)) {
			console.log("!Error! / uploadVideo / _v.is_not_valid_str(pageId)");
			return;
		}

		paramObj.url = 
		"https://graph.facebook.com/{version}/{page-id}/photos"
		.replace(/\{version\}/gi, this.FBSDKVer)
		.replace(/\{page\-id\}/gi, pageId)
		;

		this.uploadMediaFile(callback, callbackScope, paramObj);
	}  
	, uploadMediaFile:function(callback, callbackScope, paramObj) {

		if(paramObj == null) {
			console.log("!Error! / uploadMediaFile / paramObj == null");
			return;
		}
		var formUpload = paramObj.formUpload;
		if(formUpload == null) {
			console.log("!Error! / uploadMediaFile / formUpload == null");
			return;
		}
		var pageId = paramObj.pageId;
		if(_v.is_not_valid_str(pageId)) {
			console.log("!Error! / uploadMediaFile / _v.is_not_valid_str(pageId)");
			return;
		}
		var url = paramObj.url;
		if(formUpload == null) {
			console.log("!Error! / uploadMediaFile / url == null");
			return;
		}

	    var published = true;
	    if(paramObj.published != null) {
			published = paramObj.published;
	    }
		var unpublishedContentType = "";
		var scheduled_publish_time = null;
		if( !published && 
		    paramObj.unpublishedContentType != null && 
		    this.isValidUnpublishedContentType(paramObj.unpublishedContentType) ) {

			unpublishedContentType = paramObj.unpublishedContentType;
			scheduled_publish_time = paramObj.scheduled_publish_time;
		}


		if(callback == null) {
			console.log("!Error! / uploadMediaFile / callback == null");
			return;
		}
		if(callbackScope == null) {
			console.log("!Error! / uploadMediaFile / callbackScope == null");
			return;
		}

		var nextParamObj = {
			formUpload:formUpload,
			pageId:pageId, 
			description:paramObj.description, 
			published:published, 
			unpublishedContentType:unpublishedContentType,
			callback:callback,
			callbackScope:callbackScope,
			url:url,
			scheduled_publish_time:scheduled_publish_time
		};

		var _self = this;
		var nextCallback = function(paramObj) {

			console.log("uploadMediaFile / nextCallback / paramObj ::: ",paramObj);

			var formUpload = paramObj.formUpload;
			var pageId = paramObj.pageId; 
			var published = paramObj.published;
			var unpublishedContentType = paramObj.unpublishedContentType;
			var scheduled_publish_time = paramObj.scheduled_publish_time;
			var callback = paramObj.callback;
			var callbackScope = paramObj.callbackScope;
			var accessToken = paramObj.accessToken;
			var description = paramObj.description;
			if(description == null) {
				description = "";
			}

			var postParam = null;
			if(published) {

				postParam = {
					access_token:accessToken,
					published:published,
					description:description
				};

			} else if(scheduled_publish_time != null) {

				postParam = {
					access_token:accessToken,
					published:published,
					unpublished_content_type:unpublishedContentType,
					description:description,
					scheduled_publish_time:scheduled_publish_time
				};

			} else {

				postParam = {
					access_token:accessToken,
					published:published,
					unpublished_content_type:unpublishedContentType,
					description:description
				};

			}
			paramObj.postParam = postParam;

			_self.fileUpload(callback, callbackScope, paramObj);
		}
		var nextCallbackScope = this;
		this.getPageAccessTokenAsync(nextCallback, nextCallbackScope, nextParamObj);

	}

	, getMyPage:function(callback, callbackScope, paramObj) {

		this.getPageAccessTokenAsync(function(paramObj) {

			FB.api(
			"/me/accounts",
			'GET',
			{},

				function(response) {

					console.log("response :::: ",response);

					var pageList = response.data;

					callback.apply(callbackScope,[pageList]);


				} // end callback

			); // end FB api

		}, this, paramObj);

	}

	, parse_login_response:function(response) {

		if(response == null) {
			console.log("!Error! / parse_login_response / response == null");
			return;
		}

		// 1. facebook_user_id
		// 페이스북 아이디는 FACEBOOK APP당 고유한 유저 아이디를 줍니다.
		// 동일한 유저라고 하더라도 다른 FACEBOOK APP에서는 다른 유저아이디를 가집니다.
		// hosts 파일에 특정 ip에 특정 도메인이름을 지정하는 경우, 
		// 이전 동일 유저가 로그인시 남긴 fb_user_id와 이후 동일 유저가 남긴 fb_user_id가 다를 수 있습니다.
		// @ referer : http://stackoverflow.com/questions/36157309/is-users-facebook-id-always-constant-for-specific-user-that-returns-from-facebo
		var facebook_user_id = response.id;
		// 2. email
		var facebook_user_email = response.email;
		// 3. first name & last name
		// 공백 포함된 이름의 처리.
		var facebook_user_name = response.name;
		var facebook_user_name_arr = facebook_user_name.split(" ");
		var facebook_user_first_name = "";
		var facebook_user_last_name = "";

		if(1 == facebook_user_name_arr.length) {
			facebook_user_first_name = facebook_user_name_arr[0];
		} else if(1 < facebook_user_name_arr.length) {
			facebook_user_first_name = facebook_user_name_arr.slice(0, facebook_user_name_arr.length - 1).join(" ");
			facebook_user_last_name = facebook_user_name_arr[facebook_user_name_arr.length - 1];
		}

		// 4. Picture - url / Magendas로 업로드 필요.
		var facebook_profile_image = response.picture.data.url;	
		var facebook_user_gender = response.gender;	
		var facebook_user_age_range = 0;
		if(response.age_range != null && 0 < response.age_range.min) {
			facebook_user_age_range = response.age_range.min;	
		}
		var facebook_user_locale = response.locale;

		var response_parsed = 
		_param
		.get(PROPS.PARAM_SET.USER_ID_FACEBOOK,facebook_user_id)
		.get(PROPS.PARAM_SET.FACEBOOK_USER_EMAIL,facebook_user_email)
		.get(PROPS.PARAM_SET.FACEBOOK_USER_FIRST_NAME,facebook_user_first_name)
		.get(PROPS.PARAM_SET.FACEBOOK_USER_LAST_NAME,facebook_user_last_name)
		.get(PROPS.PARAM_SET.FACEBOOK_USER_PROFILE_PICTURE,facebook_profile_image)
		.get(PROPS.PARAM_SET.FACEBOOK_USER_GENDER,facebook_user_gender)
		.get(PROPS.PARAM_SET.FACEBOOK_USER_AGE_RANGE,facebook_user_age_range)
		.get(PROPS.PARAM_SET.FACEBOOK_USER_LOCALE,facebook_user_locale)
		;

		return response_parsed;
	}

}

