<?php

class TitanThumbnailManager{

	// GD 라이브러리 확인 메서드.

	// before using this class
	// PLEASE CHECK GD2 library enabled in php.ini

	// ;extension=php_gd2.dll --> extension=php_gd2.dll

	// Linux
	// phpinfo() 의 gd 에 jpeg support 확인하세요. 
	// yum install php-gd

	// Directory permission
	// $ sudo chmod 766 thumbnail

	private static $SERVICE_ROOT_PATH;
	// QuizFileManager::set_service_root_path
	public static function set_service_root_path($new_service_root_path=""){
		TitanThumbnailManager::$SERVICE_ROOT_PATH = $new_service_root_path;
	}
	public static function get_service_root_path(){
		return TitanThumbnailManager::$SERVICE_ROOT_PATH;
	}

	private static $THUMBNAIL_PATH="/images/thumbnail";
	private static $THUMBNAIL_WIDTH=400;

	public static function get_thumbnail_dir_path() {

		$real_path = $_SERVER['DOCUMENT_ROOT'];

		$thumbnail_dir_path = $real_path . "" . TitanThumbnailManager::$SERVICE_ROOT_PATH . "" . TitanThumbnailManager::$THUMBNAIL_PATH;

		$is_not_directory_exists = !file_exists($thumbnail_dir_path);
		$is_not_writable_directory = !is_writable($thumbnail_dir_path);

		if($is_not_directory_exists) {
			echo "!Error! / TitanThumbnailManager / get_quiz_file_path / \$is_not_directory_exists ::: $thumbnail_dir_path<br/>";
			return;
		}
		if($is_not_writable_directory) {
			echo "!Error! / TitanThumbnailManager / get_quiz_file_path / \$is_not_writable_directory ::: $thumbnail_dir_path<br/>";
			return;
		}

		return $thumbnail_dir_path;
	}

	public static function get_thumbnail_name($language="", $category="", $quiz_id=-1, $quiz_num="", $image_file_ext="") {

		if(empty($language)) {
			echo "!Error! / get_thumbnail_name / empty(\$language)<br/>";
			return;
		}
		if(empty($category)) {
			echo "!Error! / get_thumbnail_name / empty(\$category)<br/>";
			return;
		}
		if(empty($quiz_id)) {
			echo "!Error! / get_thumbnail_name / empty(\$quiz_id)<br/>";
			return;
		}
		if(empty($quiz_num)) {
			echo "!Error! / get_thumbnail_name / empty(\$quiz_num)<br/>";
			return;
		}
		if(empty($image_file_ext)) {
			echo "!Error! / get_thumbnail_name / empty(\$image_file_ext)<br/>";
			return;
		}

		return $language . "_" . $category . "_" . $quiz_id . "_" . $quiz_num . ".$image_file_ext";
	}


	private static $IMAGE_FILE_TYPE_JPG="jpg";
	private static $IMAGE_FILE_TYPE_JPEG="jpeg";
	private static $IMAGE_FILE_TYPE_PNG="png";
	private static $IMAGE_FILE_TYPE_GIF="gif";

	public static function has_tumbnail($language="", $category="", $quiz_id=-1, $quiz_num="", $file_type="") {

		$thumbnail_name = TitanThumbnailManager::get_thumbnail_name($language, $category, $quiz_id, $quiz_num, $file_type);
		if(empty($thumbnail_name)) {
			echo "!Error! / TitanThumbnailManager / has_tumbnail / empty(\$thumbnail_name)<br/>";
			return;
		}

		$thumbnail_dir_path = TitanThumbnailManager::get_thumbnail_dir_path();
		if(empty($thumbnail_dir_path)) {
			echo "!Error! / TitanThumbnailManager / has_tumbnail / empty(\$thumbnail_dir_path)<br/>";
			return;
		}

		// write json str file, if file exist, overwrite on it.
		$file_path = "$thumbnail_dir_path/$thumbnail_name";
		$is_not_file_exists = !file_exists($file_path);
		if($is_not_file_exists) {
			return false;
		}
		
		return true;
	}

	public static function is_valid_image_link($img_url) {

		$regex = "(https|http)\:\/\/"; // SCHEME
		preg_match("/^$regex/", $img_url, $m, PREG_OFFSET_CAPTURE);

		if(!empty($m)) {
			return true;
		}
		return false;

	}

	public static function get_image_file_type($img_url) {

		if(TitanThumbnailManager::is_jpg($img_url)) {
			return TitanThumbnailManager::$IMAGE_FILE_TYPE_JPG;
		} else if(TitanThumbnailManager::is_jpeg($img_url)) {
			return TitanThumbnailManager::$IMAGE_FILE_TYPE_JPEG;
		} else if(TitanThumbnailManager::is_png($img_url)) {
			return TitanThumbnailManager::$IMAGE_FILE_TYPE_PNG;
		} else if(TitanThumbnailManager::is_gif($img_url)) {
			return TitanThumbnailManager::$IMAGE_FILE_TYPE_GIF;
		}
		return "";
	}

	private static function is_jpg($img_url) {

		$regex = "jpg$"; // SCHEME
		preg_match("/$regex/i", $img_url, $m, PREG_OFFSET_CAPTURE);

		if(!empty($m)) {
			return true;
		}
		return false;

	}

	private static function is_jpeg($img_url) {

		$regex = "jpeg$"; // SCHEME
		preg_match("/$regex/i", $img_url, $m, PREG_OFFSET_CAPTURE);

		if(!empty($m)) {
			return true;
		}
		return false;

	}

	private static function is_png($img_url) {

		$regex = "png$"; // SCHEME
		preg_match("/$regex/i", $img_url, $m, PREG_OFFSET_CAPTURE);

		if(!empty($m)) {
			return true;
		}
		return false;

	}

	private static function is_gif($img_url) {

		$regex = "gif$"; // SCHEME
		preg_match("/$regex/i", $img_url, $m, PREG_OFFSET_CAPTURE);

		if(!empty($m)) {
			return true;
		}
		return false;

	}

	public static function make_thumbnail($language="", $category="", $quiz_id=-1, $quiz_num="", $img_src_url="") {

		$result = new stdClass();
		$result->error = "Congrats! No Errors.";

		if(empty($img_src_url)) {
			$result->error = "make_thumbnail / empty(\$img_src_url) <br/>";
			return $result;
		}

		$file_type = TitanThumbnailManager::get_image_file_type($img_src_url);
		if(empty($file_type)) {
			$result->error = "make_thumbnail / empty(\$file_type) <br/>";
			return $result;
		}

		$thumbnail_name = TitanThumbnailManager::get_thumbnail_name($language, $category, $quiz_id, $quiz_num, $file_type);
		if(empty($thumbnail_name)) {
			$result->error = 
			"!Error! / TitanThumbnailManager / has_tumbnail / empty(\$thumbnail_name)<br/>";
			return $result;
		}

		$thumbnail_dir_path = TitanThumbnailManager::get_thumbnail_dir_path();
		if(empty($thumbnail_dir_path)) {
			$result->error = 
			"!Error! / TitanThumbnailManager / has_tumbnail / empty(\$thumbnail_dir_path)<br/>";
			return $result;
		}

		$file_path = $thumbnail_dir_path . "/" . $thumbnail_name;
		$image_width = TitanThumbnailManager::$THUMBNAIL_WIDTH;

		$result_make_thumb = TitanThumbnailManager::make_thumb($img_src_url, $file_path, $image_width);
		// return $result_make_thumb;
		if(is_null($result_make_thumb) || $result_make_thumb->success == false) {
			return $result_make_thumb;
		}

		if(file_exists($file_path)) {
			$result->success = true;
			$result->thumbnail_name = $thumbnail_name;
			return $result;
		}

		$result->success = false;
		$result->error = "No thumbnail";
		return $result;
	}

	// http://unitstep.net/blog/2009/05/05/using-curl-in-php-to-access-https-ssltls-protected-sites/
	// @ Referer : https://davidwalsh.name/create-image-thumbnail-php
	public static function make_thumb($src="", $dest="", $crop_size=-1) {

		$result = new stdClass();
		$result->error = "";

		if(empty($src)) {
			$result->error = "make_thumb / empty(\$src) <br/>";
			return $result;
		}
		if(is_null($dest)) {
			$result->error = "make_thumb / is_null(\$dest) <br/>";
			return $result;
		}
		if($crop_size < 0) {
			$result->error = "make_thumb / is_null(\$crop_size < 0) <br/>";
			return $result;
		}
		$result->crop_size = $crop_size;

		// check the download file when it got over time due to its large size.
		if(!file_exists($dest)) {

			// 1. download image file from url
			$headers = TitanThumbnailManager::getHeaders($src);
			$http_code = $headers['http_code'];
			$download_content_length = $headers['download_content_length'];

			$result->src = $src;
			$result->download_content_length = $download_content_length;
			$result->headers = $headers;

			if ($http_code !== 200) {
				return $result;
			}

			// 1. download with file_get_contents
			$result_download = TitanThumbnailManager::download_file_get_content($src, $dest);
			$result->download_file_get_content = $result_download;
			if($result_download->success == false) {
				// 2. download with curl
				$result_download = TitanThumbnailManager::download_curl($src, $dest);
				$result->download_curl = $result_download;
			}
			if($result_download->success == false) {
				return $result;
			}

		}


		// read the source image 
		$download_src_img_path = $dest;
		if(TitanThumbnailManager::is_jpeg($download_src_img_path) || TitanThumbnailManager::is_jpg($download_src_img_path)) {
			$source_image = imagecreatefromjpeg($download_src_img_path);	
		} else if(TitanThumbnailManager::is_png($download_src_img_path)) {
			$source_image = imagecreatefrompng($download_src_img_path);	
		} else if(TitanThumbnailManager::is_gif($download_src_img_path)) {
			$source_image = imagecreatefromgif($download_src_img_path);
		} else {
			$result_download->success == false;
			$result->success == false;
			$result->error = "make_thumb / unsupported image type! $download_src_img_path <br/>";
			return $result;
		}
		
		// imagecreatefromstring
		if(is_null($source_image)) {
			$result->success == false;
			$result->error = "make_thumb / is_null(\$source_image) <br/>";
			return $result;
		}
		$width = intval(imagesx($source_image));
		$result->width = $width;
		if(!(0 <$width)) {
			$result->success == false;
			$result->error = "make_thumb / !(0 <\$width) <br/>";
			return $result;
		}
		$height = intval(imagesy($source_image));
		$result->height = $height;
		if(!(0 <$height)) {
			$result->success == false;
			$result->error = "make_thumb / !(0 <\$height) <br/>";
			return $result;
		}

		// $crop_size
		// find the "desired height" of this thumbnail, relative to the desired width  
		$desired_width = 0;
		$desired_height = 0;
		if($height < $width) {

			// landscape
			if($crop_size < $height) {

				$result->view_mode = "landscape";
				$desired_height = $crop_size;
				$desired_width = floor(($desired_height * $width) / $height);

				// $desired_width:$desired_height = $width:$height;

			} else {

				$desired_width = $width;
				$desired_height = $height;
			}

			$result->desired_height = $desired_height;
			$result->desired_width = $desired_width;


		} else if($width <= $height) {

			// portrait
			if($crop_size < $width) {

				$result->view_mode = "portrait";
				$desired_width = $crop_size;
				$desired_height = floor(($desired_width * $height) / $width);

			} else {

				$desired_width = $width;
				$desired_height = $height;

			}

			$result->desired_height = $desired_height;
			$result->desired_width = $desired_width;

		}
		if(!(0 <$desired_width)) {
			$result->success == false;
			$result->error = "make_thumb / !(0 <\$desired_width) <br/>";
			return $result;
		}
		if(!(0 <$desired_height)) {
			$result->success == false;
			$result->error = "make_thumb / !(0 <\$desired_height) <br/>";
			return $result;
		}

		 // create a new, "virtual" image 
		$virtual_image = imagecreatetruecolor($desired_width, $desired_height);
		
		 // copy source image at a resized size 
		imagecopyresampled($virtual_image, $source_image, 0, 0, 0, 0, $desired_width, $desired_height, $width, $height);
		
		 // create the physical thumbnail image to its destination 
		imagejpeg($virtual_image, $dest);

		$result->success = true;
		return $result;

	}

	/**
	 * Get Headers function
	 * @param str #url
	 * @return array
	 */
	private static function getHeaders($url) {
		$ch = curl_init($url);
		curl_setopt( $ch, CURLOPT_NOBODY, true );
		curl_setopt( $ch, CURLOPT_RETURNTRANSFER, false );
		curl_setopt( $ch, CURLOPT_SSL_VERIFYPEER, false ); // SSL CA validation off
		curl_setopt( $ch, CURLOPT_HEADER, false );
		curl_setopt( $ch, CURLOPT_FOLLOWLOCATION, true );
		curl_setopt( $ch, CURLOPT_MAXREDIRS, 3 );
		curl_exec( $ch );
		$headers = curl_getinfo( $ch );
		curl_close( $ch );

		return $headers;
	}

	private static function download_file_get_content($url, $path) {

		$result = new stdClass();

		$profile_Image = $url;
		$thumb_file = $path;

		$thumb_image = file_get_contents($url);
		if ($thumb_image != false) {
		    file_put_contents($path, $thumb_image);
		}

		$is_file_exists = file_exists($path);
		if (!$is_file_exists) {
			$result->reason = "!\$is_file_exists";
			$result->success = false;
			return $result;
		}

		$result->success = true;

		return $result;
	}

	/**
	 * Download
	 * @param str $url, $path
	 * @return bool || void
	 */
	private static function download_curl($url, $path) {

		$result = new stdClass();

		# open file to write
		$fp = fopen ($path, 'w+');
		# start curl
		$ch = curl_init();

		## set curl options
		curl_setopt( $ch, CURLOPT_URL, $url );
		# set return transfer to false
		// curl_setopt($ch, CURLOPT_RETURNTRANSFER, true); // return curl error handle.
		curl_setopt( $ch, CURLOPT_RETURNTRANSFER, false ); 
		curl_setopt( $ch, CURLOPT_BINARYTRANSFER, true );
		curl_setopt( $ch, CURLOPT_SSL_VERIFYPEER, false ); // SSL CA validation off
		# increase timeout to download big file
		curl_setopt( $ch, CURLOPT_CONNECTTIMEOUT, 10 );
		# write data to local file
		curl_setopt( $ch, CURLOPT_FILE, $fp );

		# execute curl
		// $result->curl_success = curl_exec( $ch );
		curl_exec( $ch );
		# close curl
		curl_close( $ch );
		# close local file
		fclose( $fp );

		if (filesize($path) > 0) {
			$result->reason = "filesize(\$path) > 0";
			$result->success = true;
			return $result;
		}

		$result->reason = "filesize(\$path) < 0";
		$result->success = false;

		return $result;

	}

} // end of class

?>
