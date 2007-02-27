<?php
// phpQLAdmin cache Application Programming Interface (API)
//
// $Id: pql_cache.inc,v 2.12 2007-02-27 12:04:41 turbo Exp $
//

// Initialize the cache 'area'
$pql_cache = new pql_cache();

// {{{ PQL_CACHE - caching class
class pql_cache {
  var $_memcache = 0;
  var $cache;

  // {{{ add(key, value)
  function add($key, $value) {
	global $_SESSION, $_SERVER;

	if(!$value)
	  return;

	if(file_exists($_SESSION["path"]."/.DEBUG_DISABLE_CACHE"))
	  return;

	if(ereg('=', $key) and !ereg('^PQL_', $key))
	  // Looks like a DN - URL decode it
	  $key = pql_format_normalize_dn($key);
	
	if(@is_array($this->cache)) {
	  // Global variable storage
	  if((($this->cache[$key] == '') or ($this->cache[$key] != $value) or ($key == 'PQL_CONF_DEBUG_ME')) and
		 !@$_SESSION["loading_resonable_defaults"])
	  {
		$this->cache[$key] = $value;

		if(file_exists($_SESSION["path"]."/.DEBUG_CACHE"))
		  echo "ADD(1): '$key' = '$value'<br>\n";
	  }
	} else {
	  // Memcache storage ('add' does NOT overwrite!)
	  $orig = $this->get($key);
	  if($orig == '') {
		// Nothing previosly known about this define, add it...
		$this->_memcache->add($key, $value);

		if(file_exists($_SESSION["path"]."/.DEBUG_CACHE"))
		  echo "ADD(2): '$key' = '$value'<br>\n";
	  } elseif((($orig != $value) or ($key == 'PQL_CONF_DEBUG_ME')) and
			   !@$_SESSION["loading_resonable_defaults"]) {
		// Previous value, old and new value differ or we want to change
		// the PQL_CONF_DEBUG_ME option - replace old value with new...
		$this->repl($key, $value);
	  }
	}
  }
// }}}

  // {{{ get(key)
  // Searches the cache for an object and returns it if found 
  function get($key) {
    global $_SESSION;
	
	if(file_exists($_SESSION["path"]."/.DEBUG_DISABLE_CACHE"))
	  return(0);
	if(!isset($key))
	  return(0);
	
	if(ereg('=', $key) and !ereg('^PQL_', $key))
	  // Looks like a DN - URL decode it
	  $key = pql_format_normalize_dn($key);
	
	if(@is_array($this->cache)) {
	  // Global variable storage
	  if(empty($this->cache[$key])) {
		// We don't have this object yet.
		if(file_exists($_SESSION["path"]."/.DEBUG_CACHE"))
		  echo "GET(1a): '$key' <b>not</b> cached!<br>";

		return(0);
	  } else {
		if(file_exists($_SESSION["path"]."/.DEBUG_CACHE"))
		  echo "GET(1b): '$key' <b>is</b> cached => '$value'<br>";
		
		return($this->cache[$key]);
	  }
	} else {
	  // Memcache storage
	  $value = $this->_memcache->get($key);
	  if(!empty($value)) {
		if(file_exists($_SESSION["path"]."/.DEBUG_CACHE"))
		  echo "GET(2a): '$key' <b>is</b> cached => '$value'<br>";
		
		return($value);
	  } else {
		if(file_exists($_SESSION["path"]."/.DEBUG_CACHE"))
		  echo "GET(2b): '$key' <b>not</b> cached!<br>";
		
		return(0);
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
	
	if(ereg('=', $key) and !ereg('^PQL_', $key))
	  // Looks like a DN - URL decode it
	  $key = pql_format_normalize_dn($key);
	
	if(@is_array($this->cache)) {
	  // Global variable storage
	  if($this->cache[$key]) {
		if(file_exists($_SESSION["path"]."/.DEBUG_CACHE"))
		  echo "DEL(1): '$key'<br>";

		unset($this->cache[$key]);
	  }
	} else {
	  // Memcache storage
	  if($this->get($key)) {
		// There is such an object - delete it
		if(file_exists($_SESSION["path"]."/.DEBUG_CACHE"))
		  echo "DEL(2): '$key'<br>";

		$this->_memcache->delete($key);
	  }
    }
  }
  // }}}

  // {{{ repl(key, value)
  function repl($key, $value) {
	if(@is_array($this->cache)) {
	  // Global variable storage
	  $this->cache[$key] = $value;

	  if(file_exists($_SESSION["path"]."/.DEBUG_CACHE"))
		echo "RPL(1): '$key' = '$value'<br>\n";
	} else {
	  if(file_exists($_SESSION["path"]."/.DEBUG_CACHE"))
		echo "RPL(2): '$key' = '$value'<br>\n";
	  $this->_memcache->replace($key, $value);  //, 1, (60*10));
	}
  }
  // }}}

  // {{{ flush(void)
  function flush() {
	if(@is_array($this->cache))
	  $this->_memcache->flush();
	else
	  $this->cache = array();
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
		@$this->_memcache->connect('localhost', 11211);
		if(@$this->_memcache)
		  return true;
		$msg = " WARNING: Connection to memcached failed. Using normal apache memory cache.\n";
	  } else
		$msg = " WARNING: Memcache module not loaded. Using normal apache memory cache.\n";
	  
	  $log = date("M d H:i:s");
	  $log .= $msg;
	  @error_log($log, 3, "phpQLadmin.log");
	  
	  // Fall back on global variable cache
	  $this->cache = array();
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
