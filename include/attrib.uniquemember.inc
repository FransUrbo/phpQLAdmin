<?php
// Manage the uniqueMember attribute
// $Id: attrib.uniquemember.inc,v 2.3 2007-07-13 11:12:05 turbo Exp $

// {{{ attribute_check(void)
function attribute_check() {
  global $LANG, $error_text;

  $error = false;
  $got_value = 0;

  for($i=0; $i < $_REQUEST['uniquemember']; $i++) {
	$attr_nr = "uniquemember_$i";
	if($_REQUEST[$attr_nr])
	  $got_value = 1;
  }
  
  if(!$got_value) {
	$error = true;
	$error_text['uniquemember'] = $LANG->_('Missing');
  }

  return($error);
}
// }}}

// {{{ attribute_print_form(void)
function attribute_print_form() {
  global $_pql, $LANG, $error_text;

  $host_reference = $_pql->get_attribute($_REQUEST["host"], pql_get_define("PQL_ATTR_CN"));

  $attrib = $_REQUEST["attrib"];
?>
  <form action="<?php echo $_SERVER["PHP_SELF"] ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?php echo $LANG->_('Add user as webserver admin')?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?php echo $LANG->_('User DN')?></td>
          <td>
            <?php echo pql_format_error_span($error_text[$_REQUEST[$attrib]]) . "\n"; ?>
<?php
  // TODO: This is a REALLY bad idea. If we have a 'huge' database
  // (hundred, thousands or more users), this will take AGES!
  // {{{ Show branches and users
  // This is almost an exact copy of the same stuff in
  // include/attrib.administrator.inc:attribute_print_form()!

  // We're giving user access to a known domain/branch
  $i = 0; $div_counter = 0;
  
  // Get all domains in the database.
  $domains = pql_get_domains();
  if(isset($domains)) {
	// Need some java stuff since I'm showing foldable branches etc.
	require("./left-head.html");

	foreach($domains as $key => $domain) {
	  // Get domain part from the DN (Example: 'dc=test,dc=net' => 'test').
	  $d = explode(',', $domain); $d = explode('=', $d[0]); $d = $d[1];

	  // Get root DN of domain/branch
	  $rootdn = pql_get_rootdn($domain);
	  
	  // Create a user search filter
	  if(pql_get_define("PQL_CONF_USER_FILTER", $_REQUEST["rootdn"]))
		$filter = pql_get_define("PQL_CONF_USER_FILTER", $_REQUEST["rootdn"]);
	  else
		$filter  = pql_get_define("PQL_CONF_REFERENCE_USERS_WITH", $_REQUEST["rootdn"])."=*";
	  
	  // Retreive all users in this branch/domain.
	  $users   = $_pql->get_dn($domain, $filter);
	  if(is_array($users)) {
		// Zero out the variables, othervise we won't get users in
		// specified domain, but also in the PREVIOUS domain shown!
		unset($cns); $links = array();
		foreach($users as $dn) {
		  $tmp = $_pql->get_attribute($dn, pql_get_define("PQL_ATTR_CN"));
		  if(is_array($tmp))
			$cns[$dn] = $tmp[0];
		  else
			$cns[$dn] = $tmp;
		}
		
		if(is_array($cns)) {
		  asort($cns);
		  foreach($cns as $dn => $cn) {
			$new   = array($cn => array($_REQUEST["attrib"], $i, $dn));
			$links = $links + $new;
			
			$i++;
		  }
		}
		
		pql_format_tree($d, 0, $links, 0);
	  }
	  
	  // This an ending for the domain tree
	  pql_format_tree_end();
	}
  }
?>

            <input type="hidden" name="<?php echo $attrib?>" value="<?php echo $i?>">
<?php
  if(isset($domains)) {
	require("./left-trailer.html");
  }
  // }}}
?>
      </th>
    </table>

    <input type="hidden" name="host"          value="<?php echo $_REQUEST["host"]?>">
    <input type="hidden" name="server"        value="<?php echo $_REQUEST["server"]?>">
    <input type="hidden" name="virthost"      value="<?php echo $_REQUEST["virthost"]?>">
    <input type="hidden" name="ref"           value="<?php echo $_REQUEST["ref"]?>">

    <input type="hidden" name="attrib"        value="<?php echo $_REQUEST["attrib"]?>">
    <input type="hidden" name="action"        value="<?php echo $action?>">
    <input type="hidden" name="view"          value="<?php echo $_REQUEST["view"]?>">
    <input type="hidden" name="submit"        value="yes">
    <br>
    <input type="submit" value="<?php echo $LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ attribute_save(type)
function attribute_save($type) {
  global $_pql, $LANG;

  $attrib = $_REQUEST["attrib"];

  // {{{ Retreive old values
  $members = $_pql->get_attribute($_REQUEST["server"], pql_get_define("PQL_ATTR_UNIQUE_GROUP"));
  if(!@$members) {
	$members = array();
  } elseif(@$members and !is_array($members)) {
	// Flat - make it an array
	$members = array($members);
  }
  // }}}

  if($type == 'del') {
	// {{{ Go through old values, remove the specific one
	for($i=0; $members[$i]; $i++) {
	  if($members[$i] != $_REQUEST["uniquemember"])
		$entry[pql_get_define("PQL_ATTR_UNIQUE_GROUP")][] = $members[$i];
	}

	// Just in case we've removed the last value
	if(!$entry)
	  $entry[pql_get_define("PQL_ATTR_UNIQUE_GROUP")] = array();
	// }}}
  } else {
	// {{{ Add user to object
	// Add previous values (from LDAP) to entry array.
	$entry[$attrib] = $members;
	
	// Add the new value(s) from value '<attrib>_<number(s)>'
	$amount = $_REQUEST[$attrib];
	for($i=0; $i < $amount; $i++) {
	  $var_name = $attrib . "_$i";
	  if($_POST[$var_name])
		$entry[$attrib][] = $_POST[$var_name];
	}
	// }}}
  }

  // {{{ Do modification
  if(pql_modify_attribute($_REQUEST["server"], '', '', $entry)) {
	$msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
								 array('what'  => $attrib));
  } else
	$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
								 array('what' => $attrib)) . ": " . pql_format_error(0);
	
	attribute_forward(urlencode($msg));
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
