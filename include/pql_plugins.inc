<?php
// ----------------------------
// pql_plugins.inc
// phpQLAdmin Application Programming Interface (API)
//
// $Id: pql_plugins.inc,v 2.3 2003-12-13 13:44:09 turbo Exp $

// Plugin register class
class pql_plugin_storage {
    var $plugins = array();
    var $cats	 = array();
    var $attribs = array();
    
	// {{{ add_plugin(name, file, cat)
    // Add an attribute plugin to the plugin storage
    function add_plugin($name, $file, $cat) {
		
		if(isset($this->plugins[$name]))
		  // plugin with this name already exists
		  return false;
		
		if($cat != "") {
			$this->plugins[$name]["cat"]  = $cat;
			$this->cats[$cat] = 1;
		}

		$this->plugins[$name]["file"] = $file;
	
		return true;
    }
	// }}}
    
	// {{{ add_attrib(attrib, name)
    // Add an attribute to a registred plugin
    function add_attrib($attrib, $name) {
		
		if(!isset($this->plugins[$name]))
		  // plugin with this name does not exist
		  return false;
		
		$this->attribs[$attrib] = $name;
		return true;
    }
	// }}}
    
	// {{{ get_plugin(attrib)
    // Returns name of plugin for $attrib
    function get_plugin($attrib) {
		if(!isset($this->attribs[$attrib]))
		  // an attribute with this name does not exist
		  return false;
		
		$name = $this->attribs[$attrib];
		return $name;
    }
	// }}}
    
	// {{{ get_plugin_filename(name)
    // Return filename of plugin for specified plugin name
    function get_plugin_filename($name) {
		if(!isset($this->plugins[$name]))
		  // plugin with this name does not exist
		  return false;
		
		return $this->plugins[$name]["file"];
    }
	// }}}
    
	// {{{ get_plugin_cat(name)
    // Returns category of plugin name
    function get_plugin_cat($name) {
		return $this->plugins[$name]["cat"];
    }
	// }}}
    
	// {{{ get_cat_plugins(cat)
    // Return all plugin names of category
    function get_cat_plugins($cat) {
		foreach($this->plugins as $name => $data) {
			if($data["cat"] == $cat) {
				$plugins[] = $name;
			}
		}
		
		return $plugins;
    }
	// }}}
    
	// {{{ !get_cats()
    // Return all registred categories
    function get_cats() {
		foreach($this->cats as $cat => $value)
		  $cats[] = $cat;
		
		return $cats;
    }
	// }}}
}

// =======================================

// {{{ pql_plugin_cat(name)
// Gets the category name of plugin
function pql_plugin_cat($name) {
    global $_pql_plugins;
    
    return $_pql_plugins->get_plugin_cat($name);
}
// }}}

// {{{ pql_plugin_get(attrib)
// Get a filename for $attrib
function pql_plugin_get($attrib) {
    global $_pql_plugins;
    
    return $_pql_plugins->get_plugin($attrib);
}
// }}}

// {{{ pql_plugin_get_catplugins(cat)
// Gets all plugin names for specified category
function pql_plugin_get_catplugins($cat) {
    global $_pql_plugins;
    
    return $_pql_plugins->get_cat_plugins($cat);
}
// }}}

// {{{ pql_plugin_get_cats()
// Get all plugin categories
function pql_plugin_get_cats() {
    global $_pql_plugins;
    
    return $_pql_plugins->get_cats();
}
// }}}

// {{{ pql_plugin_get_filename(name)
// Get all plugin names for specified category
function pql_plugin_get_filename($name) {
    global $_pql_plugins;
    
    return $_pql_plugins->get_plugin_filename($name);
}
// }}}

// {{{ pql_plugin_register(name, file, cat, attribs)
// Register a plugin for specified attribute
function pql_plugin_register($name, $file, $cat, $attribs) {
    global $_pql_plugins;

    $_pql_plugins->add_plugin($name, $file, $cat);

    if(is_array($attribs)) {
		foreach($attribs as $attrib)
		  $_pql_plugins->add_attrib($attrib, $name);
	} else
	  $_pql_plugins->add_attrib($attribs, $name);
}
// }}}

$_pql_plugins = new pql_plugin_storage();

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
