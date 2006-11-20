<?php
// control attribute plugin for
// xxx (short attribute description)
//
// ----------------------------
// attribute plugin skeleton
// ----------------------------
// use this to create your own plugins for administrating an
// attribute trough phpQLAdmin.
//
// replace the xxx prefix of each function
// with the name of the attribute. it's recommended
// that you use the attribute part in the filename for
// the function prefix because it must be unique. (e.g. for
// attrib.control.helohost.inc the function prefix is helohost)
//
//
// procedure of plugin calling (display data):
//          -------------------
//          | control_cat.php |
//          -------------------
//                   |
//     ----------------------------
//     | include this plugin file |
//     ----------------------------
//                   |
//  -----------------------------------
//  | raise function xxx_print_view() |
//  -----------------------------------
//
//
// procedure of plugin calling (edit data)
//     ------------------------------
//     | control_edit_attribute.php |
//     ------------------------------
//                   |
//      ----------------------------
//      | include this plugin file |
//      ----------------------------
//                   |
//          /-----------------\
//          | $is_submit == "" |--- yes --- raise function xxx_init() and xxx_print_form()
//          \-----------------/
//                   |
//         no ($is_submit is set)
//                   |
//    /----------------------------\
//    | raise function xxx_check() | --- return false: raise function xxx_print_form()
//    \----------------------------/
//                   |
//              return true
//                   |
//     ------------------------------
//     | raise function xxx_save()  |
//     | within xxx_save() redirect |
//     | to control_cat.php         |
//     ------------------------------
//
// Please look at other plugins to see which standard header text is recommanded.
//
// $Id: attrib.control.skeleton.inc,v 2.19.4.2 2006-11-20 19:57:38 turbo Exp $
//

// {{{ xxx_check(type)
function xxx_check($type) {
	global $error, $LANG;
	
	// form validation comes here.
	// this function returns true if the check was successful and the values
	// can be stored. it returns false, if something was wrong.

	if($_REQUEST["xxx"] == "") {
	  // Missing
	  $error["xxx"] = $LANG->_('Missing');
	  return false;
	}
	
	// use the error array to provide error description to the
	// form. the format is:
	// $error["formelement_name"] = "error description";
	
	return true;
}
// }}}

// {{{ xxx_init(void)
function xxx_init() {
	global $_pql_control;
	
	// initiate values from ldap database
	// use global vars to make the initiated values
	// available for the other functions
	
	// fetch data from ldap server
	if($host == "Global")
	  $value = pql_get_attribute($_pql_control->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"],
								 pql_get_define("PQL_ATTR_xxx"), 0, pql_get_define("PQL_ATTR_CN")."=*");
	else
	  $value = pql_get_attribute($_pql_control->ldap_linkid, $host, pql_get_define("PQL_ATTR_xxx"));
	
	if(@is_null($value))
	  return true;
}
// }}}


// {{{ xxx_print_view(host)
function xxx_print_view($host) {
	global $_pql_control, $LANG;

	// this function will display the configuration
	// stored in the ldap database.

	// Init data (must be here)
	xxx_init();

	// If you need the host FQDN
	//$cn = pql_get_attribute($_pql_control->ldap_linkid, $host, pql_get_define("PQL_ATTR_CN"));

	// set empty field to default value
	if(@empty($_REQUEST["xxx"]))
	  $_REQUEST["xxx"] = "set the default value here";
?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">xxx title</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title"><?=$LANG->_('value')?></td>
        <td class="<?=$class?>"><?php if(@$_REQUEST["xxx"]) { echo $_REQUEST["xxx"]; } else { echo '<i>'.$LANG->_('Not set').'</i>'; }?></td>
        <td class="<?=$class?>" align="right"><a href="control_edit_attribute.php?mxhost=<?=urlencode($host)?>&attrib=<?=pql_get_define("PQL_ATTR_xxx")?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change options')?>"></a></td>
      </tr>
    </th>
  </table>
<?php
}
// }}}

// {{{ xxx_print_form(void)
function xxx_print_form() {
	global $error, $LANG;

	// this function displays the form to change the values
	// for specified attribute.
?>
  <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">xxx <?=$LANG->_('title')?></th>
        <tr class="<?php pql_format_table(); ?>">
          <td class="title">xxx attribute</td>
          <td><?php echo pql_format_error_span($error["xxx"]); ?><input type="text" name="xxx" value="<?=$_REQUEST["xxx"]?>"> <?=$LANG->_('Default')?>: write default value here</td>
        </tr>

        <tr>
          <td colspan="2" class="subtitle">
            <img src="images/info.png" width="16" height="16" border="0"><?=$LANG->_('Leave the fields empty to use default configuration')?>.
          </td>
        </tr>
      </th>
    </table>

    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="type"   value="modify">
    <input type="hidden" name="attrib" value="<?=$_REQUEST["attrib"]?>">
    <input type="hidden" name="mxhost" value="<?=$_REQUEST["mxhost"]?>">
    <input type="hidden" name="view"   value="<?=$_REQUEST["view"]?>">
    <br>
    <input type="submit" value="<?=$LANG->_('Save')?>">
  </form>
<?php
}
// }}}

// {{{ xxx_save(type, host)
function xxx_save($type, $host) {
	global $_pql_control, $error, $LANG;
	$success = 0;

	// this function will save the entered data.
	// $type is most of the time "modify" so you
	// don't have to care about other types, which are only
	// used in special cases (like "add" or "del" to add or remove a sinlge
	// value in multifield attributes).

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
			if(!pql_modify_attribute($_pql_control->ldap_linkid, $host,
									 pql_get_define("PQL_ATTR_xxx"), '', $_REQUEST["xxx"]))
			  $success = 0;
		  }
		} else {
		  if(pql_modify_attribute($_pql_control->ldap_linkid, $host,
								  pql_get_define("PQL_ATTR_xxx"),
								  1, $_REQUEST["xxx"]))
			$success = 1;
		}

		if($success)
		  $msg = pql_complete_constant($LANG->_('Successfully changed %what%'),
									   array('what' => $LANG->_('xxx options')));
		else
		  $msg = pql_complete_constant($LANG->_('Failed to change %what%'),
									   array('what' => $LANG->_('xxx options'))) . ": " . pql_format_error(0);
		
		// this will redirect to control_cat.php and display the message $msg
		attribute_forward($msg);
		break;
		// }}}

   	default:
		die(pql_complete_constant($LANG->_('Unknown save type %type% in file %file%, function save()'),
								  array('type' => $type, 'file' => __FILE__)));
	}
}
// }}}


// {{{ xxx_help(void)
function xxx_help() {
	global $LANG;
?>
  <?=$LANG->_('Help about the attribute")?>
<?php
}
// }}}

// {{{ xxx_help_cr(void)
function xxx_help_cr() {
	global $LANG;
?>
	copyright notice about help text comes here
<?php
}
// }}}

// Local variables:
// mode: php
// mode: font-lock
// tab-width: 4
// End:
?>
