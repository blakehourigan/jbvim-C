#ifndef TUI_H_INCLUDED

#define TUI_H_INCLUDED

#include "../cursor/cursor.h"
#include "../editor_common.h"
#include <sys/ioctl.h>
#include <termios.h>

typedef struct ContentOffset {
  int offset;
} ContentOffset;

typedef struct EditorInfoBar {
  int col_length;
  int row;
  int command_row;
  int cursor_location_col;
  int editor_mode_col;
} EditorInfoBar;

EditorInfoBar build_info();

void tui_enable_raw_mode(struct termios *termios_p, struct termios *termios_og);

void tui_disable_raw_mode(struct termios *termios_og);

void tui_enable_alternate_buffer();

void tui_disable_alternate_buffer();
void tui_setup(struct termios *termios_og);
void tui_refresh(enum EditorMode *mode);
void tui_move_to_command(EditorInfoBar *info_bar);
void tui_exit_command(EditorInfoBar *info_bar, Cursor *prev_curs_pos);

// pub fn update_tui(editor_state : &mut EditorState) {
//   let window_inf = InformationBar::new (&terminol::get_terminal_size());
//   let mode = editor_state.editor_mode.value();
//
//   match editor_state.editor_mode {
//     EditorMode::Command => {
//       if editor_state
//         .previous_mode == EditorMode::Command {}
//       else {
//         draw_command_field(&window_inf);
//       }
//     }
//     _ => {
//       if editor_state
//         .previous_mode == EditorMode::Command {
//           cursor::restore_cursor_position();
//           editor_state.previous_mode = editor_state.editor_mode;
//         }
//       else {
//         draw_info_tui(&window_inf);
//         draw_mode(&window_inf, &mode);
//         update_cursor(editor_state);
//       }
//     }
//   }
//   io::stdout().flush().unwrap_or_else(
//       | e | panic !("io error occurred during flush: {e}"));
// }
//
// fn update_cursor(editor_state : &mut EditorState) {
//   match editor_state.editor_mode {
//     EditorMode::Insert | EditorMode::Command = > cursor::enable_bar_cursor(),
//                          _ = > cursor::enable_standard_cursor(),
//   }
// }
//
/// draws the tui information bar including green background and default
// cursor
/// position (1,1)
// void draw_info_tui(EditorInfoBar *window_inf) {
//   let cursor = Cursor::get_cursor_coords();
//   cursor::save_cursor_position();
//   cursor::move_cursor_to(window_inf.row.try_into().unwrap(), 1);
//
//   // editor_data.cursor.mode(cursor::modes::bold);
//   let color = Colors::Red as i32;
//   cursor::set_background(color);
//
//   let bar = std::iter::repeat(" ")
//                 .take(window_inf.length as usize)
//                 .collect::<String>();
//
//   write !(io::stdout(), "{}", bar)
//       .unwrap_or_else(| e | panic !("failed io operation: {e}"));
//
//   draw_cursor_location(&window_inf, color, cursor.line, cursor.col);
//
//   cursor::restore_cursor_position();
//   cursor::reset_modes();
// }

// fn draw_cursor_location(window_inf : &InformationBar, color : i32, line :
// usize,
//                         col : usize) {
//   cursor::move_cursor_to(window_inf.row.try_into().unwrap(),
//                          window_inf.cursor_location_col.try_into().unwrap(),
//                          );
//
//   cursor::set_background(color);
//
//   write !(io::stdout(), "({},{})", line, col)
//       .unwrap_or_else(| e | panic !("failed io operation: {e}"));
// }
//
// fn draw_command_field(window_inf : &InformationBar) {
//   draw_line(window_inf.command_row.try_into().unwrap(),
//             window_inf.length.try_into().unwrap(), Colors::Black as i32, );
//   cursor::move_cursor_to(window_inf.command_row.try_into().unwrap(), 1);
//
//   write !(io::stdout(), ":")
//       .unwrap_or_else(| e | panic !("failed io operation: {e}"));
// }
//
// fn draw_mode(window_inf : &InformationBar, mode : &str) {
//   cursor::save_cursor_position();
//   cursor::move_cursor_to(window_inf.command_row.try_into().unwrap(),
//                          window_inf.editor_mode_col.try_into().unwrap(), );
//
//   write !(io::stdout(), "{}", mode)
//       .unwrap_or_else(| e | panic !("failed io operation: {e}"));
//   cursor::restore_cursor_position();
// }
//
// fn draw_line(line_num : usize, length : usize, color : i32) {
//   cursor::save_cursor_position();
//   cursor::move_cursor_to(line_num, 1);
//
//   cursor::set_background(color);
//
//   let bar = std::iter::repeat(" ").take(length).collect::<String>();
//
//   write !(io::stdout(), "{}", bar).unwrap_or_else(| e | panic !("io
//   error{e}"));
//
//   cursor::restore_cursor_position();
// }
//
// pub fn update_line(line : String) {
//   cursor::save_cursor_position();
//   write !(io::stdout(), "{}", line)
//       .unwrap_or_else(| e | panic !("failed io operation: {e}"));
//   cursor::restore_cursor_position();
// }
// pub fn update_below(start : usize, end : usize, mut content : Lines) {
//   cursor::save_cursor_position();
//     for
//       i in start..end {
//         cursor::move_cursor_to(i + 1, 1);
//         let line = content.next().unwrap();
//         write !(io::stdout(), "{}", line)
//             .unwrap_or_else(| e | panic !("failed io operation: {e}"));
//       }
//     cursor::restore_cursor_position();
// }
// pub fn is_first_line() -> bool {
//   let curs_line = Cursor::get_cursor_coords().line;
//   if curs_line
//     == 1 { true }
//   else {
//     false
//   }
// }
// pub fn is_last_line() -> bool {
//   let window_inf = InformationBar::new (&terminol::get_terminal_size());
//   let curs_line = Cursor::get_cursor_coords().line;
//
//   if (window_inf.row as usize - 1)
//     == curs_line { true }
//   else {
//     false
//   }
// }
// pub fn refresh_display(direction : &str, offset : &mut ContentOffset,
//                        mut content : Lines) {
//   cursor::save_cursor_position();
//   match direction {
//     "up" => {
//       if !(offset.offset == 0) {
//         offset.offset -= 1;
//       }
//     }
//     "down" => { offset.offset += 1; }
//     _ =>(),
//   }
//     for
//       _ in 0..offset.offset { content.next(); }
//
//     let window_inf = InformationBar::new (&terminol::get_terminal_size());
//     let mut line_num : usize = 1;
//
//     for
//       line in content {
//         if (line_num as u32)
//           == window_inf.row { break; }
//         cursor::move_cursor_to(line_num, 1);
//         write !(io::stdout(), "{}", line)
//             .unwrap_or_else(| e | panic !("failed io operation: {e}"));
//         line_num += 1
//       }
//     cursor::restore_cursor_position();
// }
//
// pub fn write_existing_file(file_contents : String) {
//   let window_inf = InformationBar::new (&terminol::get_terminal_size());
//   let mut line_num : usize = 1;
//     for
//       line in file_contents.lines() {
//         // stop printing lines right before we get to the InformationBar
//         if (line_num as u32)
//           == window_inf.row - 1 { break; }
//         cursor::move_cursor_to(line_num, 1);
//         write !(io::stdout(), "{}", line)
//             .unwrap_or_else(| e | panic !("failed io operation: {e}"));
//         line_num += 1
//       }
// }

#endif
