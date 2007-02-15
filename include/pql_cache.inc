<?php
// phpQLAdmin cache Application Programming Interface (API)
//
// $Id: pql_cache.inc,v 2.10 2007-02-15 12:56:30 turbo Exp $
//

// Initialize the cache 'area'
$pql_cache = new pql_cache();

// {{{ PQL_CACHE - caching class
class pql_cache {
  var $_memcache = 0;

  // {{{ add(key, value)
  function add($key, $value) {
	global $_SESSION, $_SERVER;

	if(file_exists($_SESSION["path"]."/.DEBUG_DISABLE_CACHE"))
	  return;

	if(ereg('=.*,', $key) and function_exists("pql_maybe_encode"))
	  // Looks like a DN - URL decode it
	  $key = pql_format_normalize_dn($key);
	
	if(is_array($this)) {
	  // Global key storage
	  if((($this[$key] == '') or ($this[$key] != $value) or ($key == 'PQL_CONF_DEBUG_ME')) and
		 !@$_SESSION["loading_resonable_defaults"])
	  {
		$this[$key] = $value;

		if(file_exists("./.DEBUG_DEFINES") and ereg('_CONF_', $key) and
		   !eregi('index2\.php$', $_SERVER["SCRIPT_FILENAME"]))
		  echo "ADD: $key = '$value'<br>\n";
	  }
	} else {
	  // Memcache storage ('add' does NOT overwrite!)
	  $orig = $this->get($key);
	  if($orig == '') {
		// Nothing previosly known about this define, add it...
		$this->_memcache->add($key, $value);

		if(file_exists("./.DEBUG_DEFINES") and ereg('_CONF_', $key) and
		   !eregi('index2\.php$', $_SERVER["SCRIPT_FILENAME"]))
		  echo "ADD: $key = '$value'<br>\n";
	  } elseif((($orig != $value) or ($key == 'PQL_CONF_DEBUG_ME')) and
			   !@$_SESSION["loading_resonable_defaults"])
		// Previous value, old and new value differ or we want to change
		// the PQL_CONF_DEBUG_ME option - replace old value with new...
		$this->repl($key, $value);
	}
  }
// }}}

  // {{{ get(key)
  // Searches the cache for an object and returns it if found 
  function get($key) {
    global $_SESSION;
	
	if(file_exists($_SESSION["path"]."/.DEBUG_DISABLE_CACHE"))
	  return(1);
	if(!isset($key))
	  return(2);
	
	if(ereg('=.*,', $key) and function_exists("pql_maybe_encode"))
	  // Looks like a DN - URL decode it
	  $key = pql_format_normalize_dn($key);
	
	if(is_array($this)) {
	  if(empty($this[$key])) {
		// We don't have this object yet.
		if(file_exists($_SESSION["path"]."/.DEBUG_CACHE"))
		  echo "Object '$key' <b>not</b> cached!<br>";
		return(3);
	  } else {
		if(file_exists($_SESSION["path"]."/.DEBUG_CACHE"))
		  echo "Object '$key' <b>is</b> cached<br>";
		
		return($this[$key]);
	  }
	} else {
	  $object = $this->_memcache->get($key);
	  if(@($object != '')) {
		if(file_exists($_SESSION["path"]."/.DEBUG_CACHE"))
		  echo "Object '$key' <b>is</b> cached<br>";
		
		return($object);
	  } else {
		if(file_exists($_SESSION["path"]."/.DEBUG_CACHE"))
		  echo "Object '$key' <b>not</b> cached!<br>";
		
		return(false);
	  }
	}
  }
  // }}}

  // {{{ del(key)
  // Removes an userentry from the cache
  function del($key) {
	global $_SESSION;

	if(file_exists($_SESSION["path"]."/.DEBUG_DISABLE_CACHE"))
	  return(false);
	if(!@$key)
	  return(false);
	
	if(ereg('=.*,', $key) and function_exists("pql_maybe_encode"))
	  // Looks like a DN - URL decode it
	  $key = pql_format_normalize_dn($key);
	
	if(is_array($this)) {
	  if($this[$key])
		unset($this[$key]);
	} else {
	  if($this->get($key))
		// There is such an object - delete it
		$this->_memcache->delete($key);
    }
  }
  // }}}

  // {{{ repl(key, value)
  function repl($key, $value) {
	if(file_exists("./.DEBUG_DEFINES") and ereg('_CONF_', $key) and
	   !eregi('index2\.php$', $_SERVER["SCRIPT_FILENAME"]))
	  echo "RPL: $key = '$value'<br>\n";
	$this->_memcache->replace($key, $value);  //, 1, (60*10));
  }
  // }}}

  // {{{ flush(void)
  function flush() {
	$this->_memcache->flush();
	return(true);
  }
  // }}}

  // {{{ pql_cache(void)
  function pql_cache() {
	global $_SESSION;

	if(!file_exists($_SESSION["path"]."/.DEBUG_DISABLE_CACHE") and !@$this->_memcache) {
	  // Cache not disabled, continue...
	  if (class_exists('Memcache') and function_exists("memcache_connect")) {
		// The whole point with memcached is that it runs on localhost, so
		// the TCP/IP connection doesn't give any overhead! Change at your own
		// peril!
		$this->_memcache = new Memcache;
		$this->_memcache->connect('localhost', 11211);
		if(@$this->_memcache)
		  return true;
		$msg = " WARNING: Connection to memcached failed. Using normal apache memory cache.\n";
	  } else
		$msg = " WARNING: Memcache module not loaded. Using normal apache memory cache.\n";
	  
	  if(!@$this) {
		$log = date("M d H:i:s");
		$log .= $msg;
		@error_log($log, 3, "phpQLadmin.log");
		
		// Fall back on global variable cache
		$this = array();
	  }
	}
  }
  // }}}
}
// }}}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
