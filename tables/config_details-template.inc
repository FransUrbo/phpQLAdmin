<?php 
require($_SESSION["path"]."/include/pql_templates.inc");
$templates = pql_get_templates($_pql->ldap_linkid);

if(file_exists($_SESSION["path"]."/.DEBUG_ME")) {
  echo "_SESSION: "; printr($_SESSION);
  printr($templates);
}
?>
  <br>

  <table cellspacing="0" cellpadding="3" border="0">
<?php if(is_array($templates)) {
		$div_counter = 10; // Initialize the global counter

		for($i=0; $templates[$i]; $i++) {
		  if(is_array($templates[$i]["oldformat"])) {
			$oldformat = 1;
		  }
		  
		  $dn = urlencode($templates[$i]["dn"]);
?>
<th colspan="3" align="left"><?=pql_complete_constant($LANG->_('User template configuration - %name%'),
													  array('name' => $templates[$i]["name"]))?>

      <!-- -------------------- -->
<?php 		$class=pql_format_table(0); ?>
      <tr>
        <td class="title"><?=$LANG->_('Template description')?></td>
        <td class="<?=$class?>"><?=$templates[$i]["short"]?></td>
        <td class="<?=$class?>">
          <a href="config_edit_attribute.php?rootdn=<?=$dn?>&attrib=<?=pql_get_define("PQL_ATTR_USER_TEMPLATE_DESC_SHORT")?>&view=template"><?php if($templates[$i]["name"] != 'internal_group') { ?><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change user template description')?>"><?php } ?></a>
        </td>
      </tr>

      <!-- -------------------- -->
<?php		$class=pql_format_table(0);
			$url_tmp  = "config_edit_attribute.php?rootdn=".$dn."&attrib=".pql_get_define("PQL_ATTR_PASSWORD_SCHEMES");
			$url_tmp .= "&view=template";

			if(!$_SESSION["NEW_STYLE_QUOTA"] and ($templates[$i]["name"] != 'internal_group')) {
			if(!$templates[$i]["oldformat"]["passwordscheme"]) {
			  $txt = array('tit' => $LANG->_('Allowed password encryption scheme'),
						   'add' => $LANG->_('Add encryption scheme'),
						   'del' => $LANG->_('Delete encryption scheme'));
			  $url = array('add' => $url_tmp, 'del' => $url_tmp."&action=del");
			} else {
			  $txt = array('tit' => $LANG->_('Allowed password encryption scheme'));
			  $url = array();
			}
			pql_format_table_single($_pql->ldap_linkid, $txt, $url, $templates[$i]["passwordscheme"],
									pql_get_define("PQL_ATTR_PASSWORD_SCHEMES"), $class);
			}
?>

      <tr></tr>

      <!-- -------------------- -->
<?php		$class=pql_format_table(0);
			$url_tmp  = "config_edit_attribute.php?rootdn=".$dn."&attrib=".pql_get_define("PQL_ATTR_OBJECTCLASS_USER");
			$url_tmp .= "&view=template";

			if(!$templates[$i]["oldformat"]["userobjectclass"] and
			   ($templates[$i]["name"] != 'internal_group'))
			{
			  $txt = array('tit' => $LANG->_('Allowed user objectclasses'),
						   'add' => $LANG->_('Add objectclass'),
						   'del' => $LANG->_('Delete objectclass'));
			  $url = array('add' => $url_tmp, 'del' => $url_tmp."&action=del");
			} else {
			  $txt = array('tit' => $LANG->_('Allowed user objectclasses'));
			  $url = array();
			}
			pql_format_table_single($_pql->ldap_linkid, $txt, $url, $templates[$i]["userobjectclass"],
									pql_get_define("PQL_ATTR_OBJECTCLASS_USER"), $class);
?>

      <!-- -------------------- -->
<?php		$attribs = pql_templates_attributes($_pql->ldap_linkid, $templates[$i]["userobjectclass"]);
			if(!is_array($attribs["must"]))
			  $attribs["must"] = array("<i>None</i>");
			if(!is_array($attribs["may"]))
			  $attribs["may"]  = array("<i>None</i>");

			// Show the list of required attributes. It's always read only, so no frills.
			pql_format_table_single($_pql->ldap_linkid, array('tit' => $LANG->_('Required attributes')),
									array(), $attribs["must"], '', pql_format_table(0));

			// Show the list of allowed attributes. Also read only.
			pql_format_table_single($_pql->ldap_linkid, array('tit' => $LANG->_('Allowed attributes')),
									array(), $attribs["may"], '', pql_format_table(0));

			if($templates[$i]["name"] != 'internal_group') {
?>

      <tr>
        <td class="title"></td>
        <td class="<?=$class?>">
          <a href="config_template_del.php?rootdn=<?=$dn?>"><?=pql_complete_constant($LANG->_('Delete template %name%'), array('name' => $templates[$i]["name"]))?></a>
        </td>
        <td class="<?=$class?>"></td>
      </tr>
<?php } ?>
    </th>
<?php	} // for(templates) ?>

    <th>
      <tr>
        <td class="title"></td>
        <td class="<?=$class?>">
          <a href="config_template_add.php?rootdn=<?=$_SESSION["BASE_DN"][0]?>"><?=$LANG->_('Add template')?></a>
        </td>
        <td class="<?=$class?>"></td>
      </tr>
<?php	if($oldformat) { ?>

      <tr>
        <td class="title"></td>
        <td class="<?=$class?>">
          <a href="scripts/update_templates.php?rootdn=<?=$_SESSION["BASE_DN"][0]?>"><?=$LANG->_('Update templates')?></a>
        </td>
        <td class="<?=$class?>"></td>
      </tr>
<?php	} ?>
    </th>
<?php  } else {
		$class=pql_format_table(0);
?>
    <th>
      <tr>
        <td class="title"><?=$LANG->_('No templates defined')?></td>
        <td class="<?=$class?>">
          <a href="config_template_add.php?rootdn=<?=$_SESSION["BASE_DN"][0]?>"><?=$LANG->_('Add template')?></a>
        </td>
      </tr>
    </th>
<?php } ?>
  </table>

<?php if($oldformat) { ?>
  <table cellspacing="0" cellpadding="3" border="0">
    <th>
      <tr>
        <td class="title" align="right"><img src="images/info.png" width="16" height="16" alt="" border="0"></td>
        <td class="<?=$class?>"><?=pql_complete_constant($LANG->_("The old ways for phpQLAdmin to do things with users was to have some information in the root dn (\b%rootdn%\B in your case). But the new way is to keep that information in specific objects below the '\bou=Templates,%rootdn%\B' branch (created if it doesn't exists already).<br>I see that you still have information in your root dn. To make phpQLAdmin work at it's best, please update your database.<br>I can't let you change stuff that's retreived from %rootdn%, so if you'd like to change any of that, please update your database."), array('rootdn' => $_SESSION["BASE_DN"][0]))?></td>
      </tr>
    </th>
  </table>
<?php }

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
