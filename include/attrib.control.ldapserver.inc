<?php
// control attribute plugin for
// ldapserver (server which holds ldap database)
// ldapbasedn (dn, which points to ldap database)
// ldaplogin (dn of a database user)
// ldappassword (password for bind - used with ldaplogin)
// ldapgrouplogin (dn of a database user - used by qmail-group)
// ldapgrouppassword (password for bind - used with ldapgrouplogin)
//
// $Id: attrib.control.ldapserver.inc,v 2.21.4.2 2006-11-20 19:55:00 turbo Exp $

// {{{ ldapserver_check(type)
function ldapserver_check($type) {
	global $error, $LANG;
	$attrib = $_REQUEST["attrib"];

	if(($attrib == 'ldapserver') and @is_array($_REQUEST["ldapserver"])) {
	  // check for valid fqdn for all servers
	  for($i=0; $i < count($_REQUEST["ldapserver"]); $i++) {
		$server = $_REQUEST["ldapserver"][$i];

		if(eregi('^ldap://', $server)) {
		  $uri = "ldap://";
		  $server = eregi_replace($uri, '', $server);
		} elseif(eregi('^ldaps://', $server)) {
		  $uri = "ldaps://";
		  $server = eregi_replace($uri, '', $server);
		} elseif(eregi('^ldapi://', $server)) {
		  $uri = "ldapi://";
		  
		  $server = eregi_replace($uri, '', $server);

		  if(eregi('^/', $server))
			$_REQUEST["ldapserver"][$i] = $uri.urlencode($server);
		}

		if(($uri == "ldap://") or ($uri == "ldaps://")) {
		  if(!empty($server) and !pql_check_hostaddress($server, true)) {
			$error["ldapserver_$i"] = $LANG->_('Invalid');
			$is_error = true;
		  }
		}
	  }

	  if(($uri == "ldap://") or ($uri == "ldaps://")) {
		// test connection
		if(($_REQUEST["ldap_test"] == "1") and ($is_error == false)) {
		  foreach($_REQUEST["ldapserver"] as $server) {
			if(empty($server))
			  continue;
			
			$linkid = @ldap_connect($server);
			if($linkid) {
			  if($_REQUEST["ldaplogin_use"] == "1") {
				if(!@ldap_bind($linkid, $_REQUEST["ldaplogin"], $_REQUEST["ldappassword"])) {
				  $error["ldap_test"] .= pql_complete_constant($LANG->_('Failed running a test connection to %server%'),
															   array('server' => $server)) . ": " . ldap_error($linkid) . "<br>";
				  $is_error = true;
				}
			  } else {
				if(!@ldap_bind($linkid)) {
				  $error["ldap_test"] .= pql_complete_constant($LANG->_('Failed running a test connection to %server%'),
															   array('server' => $server)) . ": " . ldap_error($linkid) . "<br>";
				  $is_error = true;
				}
			  }
			} else {
			  // connection failed
			  $error["ldap_test"] .= pql_complete_constant($LANG->_('Failed running a test connection to %server%'),
														   array('server' => $server))
				. '(' . $LANG->_('server not responding or invalid name') . ')<br>';
			  $is_error = true;
			}
			
			// search for base dn
			//if($is_error == false) 
			// TODO: How to check basedn???
			
			@ldap_unbind($linkid);
			@ldap_close($linkid);
		  }
		}
	  }
	} elseif(!@$_REQUEST[$attrib]) {
	  // missing
	  $error[$attrib] = $LANG->_('Missing');
	  $is_error = true;
	}
	
	if($is_error == false)
	  return true;
}
// }}}

// {{{ ldapserver_init(host)
// init values
function ldapserver_init($host) {
	global $_pql_control;
	
	// fetch data from ldap server
	$attribs = array("ldapserver"   	=> pql_get_define("PQL_ATTR_LDAPSERVER"),
					 "ldapbasedn"   	=> pql_get_define("PQL_ATTR_LDAPBASEDN"),
					 "ldaplogin"    	=> pql_get_define("PQL_ATTR_LDAPLOGIN"),
					 "ldappassword"		=> pql_get_define("PQL_ATTR_LDAPPASSWORD"),
					 "ldapgrouplogin"	=> pql_get_define("PQL_ATTR_GROUP_LOGIN"),
					 "ldapgrouppassword"=> pql_get_define("PQL_ATTR_GROUP_PASSWD"));
	if($host == "Global")
	  foreach($attribs as $key => $attrib)
		$_REQUEST[$key] = pql_get_attribute($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
											$attrib, 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  foreach($attribs as $key => $attrib)
		$_REQUEST[$key] = pql_get_attribute($_pql_control->ldap_linkid, $host, $attrib);
	
	// init checkbox
	if(@$_REQUEST["ldaplogin"])
	  $_REQUEST["ldaplogin_use"] = 1;
	
	// map different hostnames sperated by space to an array
	$_REQUEST["ldapserver"] = split(" ", $_REQUEST["ldapserver"]);
	if(@empty($_REQUEST["srvcount"]))
	  $_REQUEST["srvcount"] = count($_REQUEST["ldapserver"]) - 1;
}
// }}}


// {{{ ldapserver_print_view(host)
function ldapserver_print_view($host) {
	global $_pql_control, $LANG;

	// init data
	ldapserver_init($host);

	// don't display passwords
	if(!@empty($_REQUEST["ldappassword"]))
	  $_REQUEST["ldappassword"] = $LANG->_('Set, but now shown');

	// set empty field to "not set"
	$attribs = array("ldapserver"   	=> pql_get_define("PQL_ATTR_LDAPSERVER"),
					 "ldapbasedn"   	=> pql_get_define("PQL_ATTR_LDAPBASEDN"),
					 "ldaplogin"    	=> pql_get_define("PQL_ATTR_LDAPLOGIN"),
					 "ldappassword" 	=> pql_get_define("PQL_ATTR_LDAPPASSWORD"),
					 "ldapgrouplogin"	=> pql_get_define("PQL_ATTR_GROUP_LOGIN"),
					 "ldapgrouppassword"=> pql_get_define("PQL_ATTR_GROUP_PASSWD"));
	foreach($attribs as $key => $attrib) {
		if(@empty($_REQUEST[$key]))
		  $_REQUEST[$key] = '<i>'.$LANG->_('Not set').'</i>';
	}

	// Setup the tabled list of LDAP servers
	$class   = pql_format_table(0);
	$url_tmp = "control_edit_attribute.php?mxhost=$host&attrib=".pql_get_define("PQL_ATTR_LDAPSERVER");

	$tbl_txt = array('tit' => $LANG->_('LDAP Servers'), 'add' => $LANG->_('Add server'), 'del' => $LANG->_('Remove server'));
	$tbl_url = array('add' => $url_tmp.'&type=add', 'del' => $url_tmp.'&type=del&submit=1');
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?=$LANG->_('Server connection options')?></th>
<?php pql_format_table_single($_pql_control->ldap_linkid, $tbl_txt, $tbl_url, $_REQUEST["ldapserver"], pql_get_define("PQL_ATTR_LDAPSERVER"), $class); ?>
<?php $class=pql_format_table(0); ?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Base DN')?></td>
        <td><?=$_REQUEST["ldapbasedn"]?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_LDAPBASEDN")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change option').' '.pql_get_define("PQL_ATTR_LDAPBASEDN")?>"></a></td>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Login')?></td>
        <td><?=$_REQUEST["ldaplogin"]?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_LDAPLOGIN")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change option').' '.pql_get_define("PQL_ATTR_LDAPLOGIN")?>"></a></td>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Password')?></td>
        <td><?=$_REQUEST["ldappassword"]?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_LDAPPASSWORD")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change option').' '.pql_get_define("PQL_ATTR_LDAPPASSWORD")?>"></a></td>
      </tr>

<?php if($_REQUEST["ldapserver"] == $LANG->_('Not set') or $_REQUEST["ldapbasedn"] == $LANG->_('Not set')) { ?>
<?php $class=pql_format_table(0); ?>
      <tr class="subtitle">
        <td colspan="3"><span class="error"><?=$LANG->_('You have to set both, host and base dn to make qmail-ldap work')?>.</td>
      </tr>

<?php } ?>
<?php $class=pql_format_table(0); ?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Group login')?></td>
        <td><?=$_REQUEST["ldapgrouplogin"]?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_GROUP_LOGIN")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change option').' '.pql_get_define("PQL_ATTR_GROUP_LOGIN")?>"></a></td>
      </tr>

<?php $class=pql_format_table(0); ?>
      <tr class="<?=$class?>">
        <td class="title"><?=$LANG->_('Group login password')?></td>
        <td><?=$_REQUEST["ldapgrouppassword"]?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=<?=pql_get_define("PQL_ATTR_GROUP_PASSWD")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change option').' '.pql_get_define("PQL_ATTR_GROUP_PASSWD")?>"></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ ldapserver_print_form(void)
function ldapserver_print_form() {
	global $error, $LANG;
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$LANG->_('LDAP server connection option')?></th>
<?php if($_REQUEST["attrib"] == 'ldapserver') {
	 	for($i=0; $i < count($_REQUEST["ldapserver"]); $i++) { ?>
	<input type="hidden" name="ldapserver[<?=$i?>]" value="<?=$_REQUEST["ldapserver"][$i]?>">
<?php 	} ?>

        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('LDAP Server')?></td>
          <td>
            <?php echo pql_format_error_span($error["ldapserver"][$i]); ?>
            <input type="text" name="ldapserver[<?=$i?>]" size="40">
          </td>
        </tr>

<?php } if(($_REQUEST["attrib"] == 'ldapbasedn') or !is_array($_REQUEST["ldapserver"])) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Base DN')?></td>
          <td><?php echo pql_format_error_span($error["ldapbasedn"]); ?><input type="text" name="ldapbasedn" value="<?=$_REQUEST["ldapbasedn"]?>"></td>
        </tr>

<?php } if(!is_array($_REQUEST["ldapserver"])) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td colspan="2"><input type="checkbox" name="ldaplogin_use" value="1" <?php if($_REQUEST["ldaplogin_use"] == "1"){ echo "checked"; } ?>> <?=$LANG->_('Use login and password')?></td>
        </tr>

<?php } if(($_REQUEST["attrib"] == 'ldaplogin') or !is_array($_REQUEST["ldapserver"])) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Login (valid dn)')?></td>
          <td><?php echo pql_format_error_span($error["ldaplogin"]); ?><input type="text" name="ldaplogin" value="<?=$_REQUEST["ldaplogin"]?>" size="80"></td>
        </tr>

<?php } if(($_REQUEST["attrib"] == 'ldappassword') or !is_array($_REQUEST["ldapserver"])) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Password')?></td>
          <td><?php echo pql_format_error_span($error["ldappassword"]); ?><input type="password" name="ldappassword" value="<?=$_REQUEST["ldappassword"]?>"></td>
        </tr>

<?php } if(($_REQUEST["attrib"] == 'ldapgrouplogin') or !is_array($_REQUEST["ldapserver"])) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Group login')?></td>
          <td><?php echo pql_format_error_span($error["ldapgrouplogin"]); ?><input type="text" name="ldapgrouplogin" size="80"></td>
        </tr>

<?php } if(($_REQUEST["attrib"] == 'ldapgrouppassword') or !is_array($_REQUEST["ldapserver"])) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title"><?=$LANG->_('Group login password')?></td>
          <td><?php echo pql_format_error_span($error["ldapgrouppassword"]); ?><input type="password" name="ldapgrouppassword"></td>
        </tr>

<?php } ?>
<?php if($_REQUEST["attrib"] == 'ldapserver') { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td colspan="2"><input type="checkbox" name="ldap_test" value="1" <?php if($_REQUEST["ldap_test"] == "1") { echo "checked"; }?>>
			<?=$LANG->_('Test connection after submitting form (it will be tested for each server seperatly)')?>
          </td>
        </tr>

<?php } ?>
<?php if(!@empty($error["ldap_test"])) { ?>
        <tr class="<?php pql_format_table(); ?>">
          <td colspan="2"><?php echo pql_format_error_span($error["ldap_test"]); ?></td>
        </tr>
<?php } ?>
      </th>
    </table>

    <input type="hidden" name="submit"   value="1">
    <input type="hidden" name="type"     value="modify">
    <input type="hidden" name="attrib"   value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost"   value="<?=$_REQUEST["mxhost"]?>">
    <input type="hidden" name="view"     value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ ldapserver_save(type, host)
function ldapserver_save($type, $host) {
	global $_pql_control, $error, $LANG;
	$success = 0;
	$attrib = $_REQUEST["attrib"];
	
	if($attrib == 'ldapserver') {
	  // map ldapserver array to a single string
	  $ldapserver = trim(join(" ", $_REQUEST["ldapserver"]));
	  
	  // remove all double, triple or whatever spaces, replace with single one
	  $ldapserver = preg_replace("/\s+/", " ", $ldapserver);

	  // ADD a server to existing list
	  $old = '';
	} else {
	  // REPLACE value
	  $old = 1;
	}

	switch($type) {
		case "modify":
		case "add":
		  // {{{ save entries
		  if($host == "Global") {
			// Get all QLC host
			$qlcs = pql_get_dn($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
							   '(&(cn=*)(objectclass=qmailControl))');
			
			$success = 1;
			foreach($qlcs as $host) {
			  if(!pql_modify_attribute($_pql_control->ldap_linkid, $host, $attrib, '', $_REQUEST[$attrib]))
				$success = 0;
			}
		  } else {
			if(pql_modify_attribute($_pql_control->ldap_linkid, $host,
									$attrib, $old, $_REQUEST[$attrib]))
			  $success = 1;
		  }

		  if($success)
			$msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
										 array('what' => $LANG->_('ldapserver options')));
		  else
			$msg = pql_complete_constant($LANG->_('Failed to change %what%'),
										 array('what' => $LANG->_('ldapserver options')))
			  . ": " . pql_format_error(0);
		  
		  attribute_forward($msg);
		  break;
		  // }}}

		default:
		  die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
									array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ ldapserver_help(void)
function ldapserver_help() {
	global $LANG;
?>
<?php if($_REQUEST["attrib"] == 'ldapserver') { ?>
  <b><?=$LANG->_('Host')?></b><br>
	Your ldap server's hostname. If you want more than one ldap-server for redundancy,
	use the "add more servers" link to add another server. To clear additional server simply
	remove the value and save.<p>
    <b>NOTE 1:</b> It is possible to use URI in the hostname field (such as <u>ldaps://ldap.domain.tld</u>). If using
    LDAPI, then the path to the socket can be either URL encoded (such as <u>ldapi://%2Fvar%2Frun%2Fldapi</u>) or
    not encoded (such as <u>ldapi:///var/run/ldapi</u> - phpQLAdmin will automatically URL encode the value for you).<br>
    <b>NOTE 2:</b> If using LDAPI, the connection will <u><i>not</i></u> be tested!
<?php } if($_REQUEST["attrib"] == 'ldapbasedn') { ?>
  <b><?=$LANG->_('Base DN')?></b><br>
    The BaseDN for ldap searches. See OpenLDAP's documentation for more information about BaseDN. Required.
    <br>
    <i>Example: <u>o=yourcorp,c=ch</u></i>
<?php } if($_REQUEST["attrib"] == 'ldaplogin') { ?>
  <b><?=$LANG->_('Login')?></b><br>
	If you need to authentificate against your ldap server to retrieve the user information, this is the username to do so.
    <br>
	<i>Note1: This is a ldap dn, not a unix username.</i>
    <br>
	<i>Note2: If unset, all searches will be done anonymously so make sure everyone can read the relevant values.</i>
<?php } if($_REQUEST["attrib"] == 'ldappassword') { ?>
  <b><?=$LANG->_('Password')?></b><br>
	The password for the user defined in ldaplogin if needed.
    <br>
    <i>Note: This should be entered as cleartext, not encrypted!</i>
<?php } if($_REQUEST["attrib"] == 'ldapgrouplogin') { ?>
  <b><?=$LANG->_('Group login')?></b><br>
	If qmail-group need to authentificate against your ldap server to retrieve the user information, this is the username
    to do so.
    <br>
	<i>Note1: This is a ldap dn, not a unix username.</i>
    <br>
	<i>Note2: If unset, it will default to the value in ldaplogin.</i>
<?php } if($_REQUEST["attrib"] == 'ldapgrouppassword') { ?>
  <b><?=$LANG->_('Group login password')?></b><br>
	The password for the user defined in ldapgrouplogin if needed.
    <br>
    <b>NOTE 1:</b> This should be entered as cleartext, not encrypted!</i>
    <br>
	<b>NOTE 2:</b> If unset, it will default to the value in ldappassword.</i>
<?php }
}
// }}}

// {{{ ldapserver_help_cr(void)
function ldapserver_help_cr() {
	global $LANG;
	echo $LANG->_('Help text taken from life with qmail-ldap');
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
