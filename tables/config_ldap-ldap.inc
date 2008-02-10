<?php
// {{{ Setup sub-buttons
if($_REQUEST["view"] == 'databases') {
  for($i = 0; $configs[$i]; $i++) {
    if($configs[$i][pql_get_define("PQL_ATTR_OLC_DB_SUFFIX")])
      $name = $configs[$i][pql_get_define("PQL_ATTR_OLC_DB_SUFFIX")];
    else
      $name = $configs[$i][pql_get_define("PQL_ATTR_OLC_DB")];

    if($_REQUEST["database"] == $name)
      $mark = "&mark=yes";
    else
      unset($mark);
?>
    <a href="<?php echo $_SERVER["PHP_SELF"]."?view=databases&type=config&database=$name"?>"><img alt="// <?php echo $name?> \\" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?php echo $name?><?php echo $mark?>"></a>
<?php
  }

  echo "    <br><br>";
} elseif($_REQUEST["view"] == 'overlays') {
  for($i = 0; $configs[$i]; $i++) {
    $name = $configs[$i][pql_get_define("PQL_ATTR_OLC_OVERLAY")];

    if($_REQUEST["overlay"] == $name)
      $mark = "&mark=yes";
    else
      unset($mark);
?>
    <a href="<?php echo $_SERVER["PHP_SELF"]."?view=overlays&type=config&overlay=$name"?>"><img alt="// <?php echo $name?> \\" vspace="0" hspace="0" border="0" src="tools/navbutton.php?label=<?php echo $name?><?php echo $mark?>"></a>
<?php
  }

  echo "    <br><br>";
}
// }}}

// {{{ Primary buttons
?>

<?php if(!$_REQUEST["view"] or ($_REQUEST["view"] == 'default')) { ?>
      <th colspan="3" align="left"><?php echo $LANG->_('Global configuration')?>
<?php } elseif($_REQUEST["view"] == 'databases') { ?>
      <th colspan="3" align="left"><?php echo $LANG->_('Database configuration')?> - <?php echo $_REQUEST["database"]?>
<?php } elseif($_REQUEST["view"] == 'overlays') { ?>
      <th colspan="3" align="left"><?php echo $LANG->_('Overlay configuration')?> - <?php echo $_REQUEST["overlay"]?>
<?php }
// }}}

foreach($attributes as $type => $attrs) {
  asort($attrs);
  foreach($attrs as $attr) {
    if(($attr == pql_get_define("PQL_ATTR_OLC_SASL_SEC_PROPS")) and
       !is_array($conf[$attr]) and
       preg_match('/,/', $conf[$attr])) {
      $conf[$attr] = split(',', $conf[$attr]);
    }

    // ------------------------
    if(is_array($conf[$attr])) {
      $done_tr = 0;

//      asort($conf[$attr]);
      foreach($conf[$attr] as $val) {
        $class = pql_format_table(0);
?>
        <tr>
          <td class="title"><?php if(!$done_tr) { echo $attr; }?></td>
          <td class="<?php echo $class?>"><b><?php echo $val?>&nbsp;</b></td>
          <td class="<?php echo $class?>"></td>
        </tr>

<?php
        $done_tr = 1;
      }
    } else {
      $class = pql_format_table(0);
?>
        <tr>
          <td class="title"><?php echo $attr?></td>
          <td class="<?php echo $class?>"><b><?php echo $conf[$attr]?>&nbsp;</b></td>
          <td class="<?php echo $class?>"></td>
        </tr>

<?php
    }
  }
}
?>
        <tr>
          <td></td>
          <td></td>
          <td></td>
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
