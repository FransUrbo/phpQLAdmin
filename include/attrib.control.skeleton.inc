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
// $Id: attrib.control.skeleton.inc,v 2.6 2003-05-06 07:52:59 turbo Exp $
//

function xxx_check($type) {
	global $error, $xxx;
	
	// form validation comes here.
	// this function returns true if the check was successful and the values
	// can be stored. it returns false, if something was wrong.
	
	// use the error array to provide error description to the
	// form. the format is:
	// $error["formelement_name"] = "error description";
	
	return true;
}

// init values
function xxx_init() {
	global $_pql_control, $xxx;
	
	// initiate values from ldap database
	// use global vars to make the initiated values
	// available for the other functions
	
	// fetch data from ldap server
	$value = pql_control_get_attribute($_pql_control->ldap_linkid, $GLOBALS["USER_SEARCH_DN_CTR"], "xxx");
	
	if(!is_null($value))
	  $xxx = $value[0];
}


function xxx_print_view($host) {
	global $_pql_control, $xxx;

	// this function will display the configuration
	// stored in the ldap database.

	// init data (must be here)
	xxx_init();

	// set empty field to default value
	if($xxx == ""){
		$xxx = "set the default value here";
 	}

?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">XXX title</th>
      <tr class="<?php table_bgcolor(); ?>">
        <td class="title">XXX attribute</td>
        <td><?php echo $helohost; ?></td>
      </tr>

      <tr class="subtitle">
        <td colspan="2">
          <a href="control_edit_attribute.php?mxhost=<?=$host?>&attrib=xxx">
            <img src="images/edit.png" width="12" height="12" border="0"> change options
          </a>
        </td>
      </tr>
    </th>
  </table>

<?php
}

function xxx_print_form() {
	global $attrib, $xxx, $PHP_SELF, $error, $mxhost;

	// this function displays the form to change the values
	// for specified attribute.

?>
  <form action="<?php echo $PHP_SELF ?>" method="post">
    <table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left">XXX title</th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title">XXX attribute</td>
          <td><?php echo format_error($error["xxx"]); ?><input type="text" name="xxx" value="<?php echo $xxx; ?>"> Default: write default value here</td>
        </tr>

        <tr>
          <td colspan="2" class="subtitle">
            <img src="images/info.png" width="16" height="16" border="0">Leave the fields empty to use default configuration.
          </td>
        </tr>
      </th>
    </table>
    <input type="hidden" name="submit" value="1">
    <input type="hidden" name="attrib" value="<?php echo $attrib; ?>">
    <input type="hidden" name="mxhost" value="<?=$mxhost?>">
    <input type="submit" value="<?php echo PQL_LANG_SAVE; ?>">
  </form>

<?php
}

function xxx_save($type, $host) {
	global $_pql_control, $attrib, $xxx, $error;

	// this function will save the entered data.
	// $type is most of the time "modify" so you
	// don't have to care about other types, which are only
	// used in special cases (like "add" or "del" to add or remove a sinlge
	// value in multifield attributes).

	// recontrol values
	if(!helohost_check($type))
	  return false;

	switch($type) {
	  case "modify";
	  case "add";
		// save entries
		
		if(pql_control_replace_attribute($_pql_control->ldap_linkid, $GLOBALS["USER_SEARCH_DN_CTR"], "xxx", $xxx)){
			$msg = "Successfully saved xxx options";
		} else {
			$msg = "Failed saving xxx: " . pql_ldap_error($_pql_control->ldap_linkid);
    	}

		// this will redirect to control_cat.php and display the message $msg
		attribute_forward($msg);
      break;

   	default:
	  die("unknown save type $type in " . __FILE__ . ", function save()");
	}
}

function xxx_help() {
?>
	Help about the attribute...
<?php
}

function xxx_help_cr(){
?>
	copyright notice about help text comes here
<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
}
