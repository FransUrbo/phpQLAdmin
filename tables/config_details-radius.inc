<?php
// Plugin for RADIUS Policies
// $Id: config_details-radius.inc,v 1.3 2007-09-29 21:15:15 turbo Exp $
require($_SESSION["path"]."/include/texts.radius.inc");
require($_SESSION["path"]."/include/pql_radius.inc");
$radprofiles = pql_get_radprofiles();
?>
  <br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left"><?php echo $LANG->_('Global RADIUS Policies')?>
<?php for($i=0; $radprofiles[$i]; $i++) { ?>
<?php	foreach($table_columns as $attrib => $text) {
	  $link = 'config_edit_attribute.php?attrib='.$attrib;
	  $class=pql_format_table(0);
?>
      <tr>
        <td class="title"><?php echo $text?></td>
<?php if($attrib == sprintf("%s", pql_get_define("PQL_ATTR_CN"))) { ?>
        <td class="title">
<?php } else { ?>
        <td class="<?php echo $class?>">
<?php } ?>
          <?php echo $radprofiles[$i][$attrib]?>
        </td>

        <td class="<?php echo $class?>" align="right">
          <a href="<?php echo $link?>"><img src="images/edit.png" width="12" height="12" border="0" alt="<?=$LANG->_('Change profile value')?>"></a>
        </td>
      </tr>
<?php	} ?>

      <tr><td colspan="3"></td></tr>

<?php } ?>
      <tr>
<?php if(!$radprofiles) { ?>
        <td class="title"><?php echo $LANG->_('No profiles defined')?></td>
<?php } else { ?>
        <td class="title"></td>
<?php }

      $class=pql_format_table(0);
?>
        <td class="<?php echo $class?>" colspan="2">
          <a href="config_radius_add.php?rootdn=<?php echo urlencode($_SESSION["BASE_DN"][0])?>"><?=$LANG->_('Add profile')?></a>
        </td>
      </tr>
    </th>
  </table>
<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
