import glob, os, bpy, mathutils, math, time, re, os.path

wait = 1
zoom = -50
path = "C:\Repos\STL"

def replace_right(source, target, replacement, replacements=None):
    return replacement.join(source.rsplit(target, replacements))

def look_at(viewMatrix): 
    for area in bpy.context.screen.areas:
        if area.type == 'VIEW_3D':
            rv3d = area.spaces[0].region_3d
            if rv3d is not None:
                if area.spaces[0].local_view is not None:
                    bpy.ops.view3d.localview({'area': area})
                rv3d.view_perspective = 'ORTHO'
                rv3d.view_distance = 150
                rv3d.view_matrix = viewMatrix

os.chdir(path)

for file in glob.glob("*.stl"):
    bpy.ops.object.select_all(action='SELECT')
    bpy.ops.object.delete() 

    fileMesh = file
    bpy.ops.import_mesh.stl(filepath=path + fileMesh, filter_glob="*.stl", files=[{"name":fileMesh, "name":fileMesh}], directory=path)

    look_at(((-1.0, 0.0, 0.0, 0.0),
               (0.0, 0.0, 1.0, 0.0),
               (0.0, 1.0, 0.0, 0.0),
               (0.0, 0.0, zoom, 1.0)))
    bpy.ops.wm.redraw_timer(type='DRAW_WIN_SWAP', iterations=1)
    time.sleep(wait)
    bpy.ops.screen.screenshot(filepath=path + "\\" + file + "_front.jpg", hide_props_region=True, check_existing=True, filter_blender=False, filter_backup=False,
                          filter_image=True, filter_movie=False, filter_python=False, filter_font=False, filter_sound=False,
                          filter_text=False, filter_archive=False, filter_btx=False, filter_collada=False, filter_alembic=False,
                          filter_usd=False, filter_volume=False, filter_folder=True, filter_blenlib=False, filemode=9, show_multiview=False,
                          use_multiview=False, display_type='DEFAULT', sort_method='FILE_SORT_ALPHA', full=True)

    look_at(((1.0, 0.0, 0.0, 0.0),
               (0.0, 0.0, -1.0, 0.0),
               (0.0, 1.0, 0.0, 0.0),
               (0.0, 0.0, zoom, 1.0)))
    bpy.ops.wm.redraw_timer(type='DRAW_WIN_SWAP', iterations=1)
    time.sleep(wait)
    bpy.ops.screen.screenshot(filepath=path + "\\" + file + "_back.jpg", hide_props_region=True, check_existing=True, filter_blender=False, filter_backup=False,
                          filter_image=True, filter_movie=False, filter_python=False, filter_font=False, filter_sound=False,
                          filter_text=False, filter_archive=False, filter_btx=False, filter_collada=False, filter_alembic=False,
                          filter_usd=False, filter_volume=False, filter_folder=True, filter_blenlib=False, filemode=9, show_multiview=False,
                          use_multiview=False, display_type='DEFAULT', sort_method='FILE_SORT_ALPHA', full=True)
                          
    look_at(((0.0, 0.0, 1.0, 0.0),
               (1.0, 0.0, 0.0, 0.0),
               (0.0, 1.0, 0.0, 0.0),
               (0.0, 0.0, zoom, 1.0)))
    bpy.ops.wm.redraw_timer(type='DRAW_WIN_SWAP', iterations=1)
    time.sleep(wait)
    bpy.ops.screen.screenshot(filepath=path + "\\" + file + "_right.jpg", hide_props_region=True, check_existing=True, filter_blender=False, filter_backup=False,
                          filter_image=True, filter_movie=False, filter_python=False, filter_font=False, filter_sound=False,
                          filter_text=False, filter_archive=False, filter_btx=False, filter_collada=False, filter_alembic=False,
                          filter_usd=False, filter_volume=False, filter_folder=True, filter_blenlib=False, filemode=9, show_multiview=False,
                          use_multiview=False, display_type='DEFAULT', sort_method='FILE_SORT_ALPHA', full=True)

    look_at(((0.0, 0.0, -1.0, 0.0),
               (-1.0, 0.0, 0.0, 0.0),
               (0.0, 1.0, 0.0, 0.0),
               (0.0, 0.0, zoom, 1.0)))
    bpy.ops.wm.redraw_timer(type='DRAW_WIN_SWAP', iterations=1)
    time.sleep(wait)
    bpy.ops.screen.screenshot(filepath=path + "\\" + file + "_left.jpg", hide_props_region=True, check_existing=True, filter_blender=False, filter_backup=False,
                          filter_image=True, filter_movie=False, filter_python=False, filter_font=False, filter_sound=False,
                          filter_text=False, filter_archive=False, filter_btx=False, filter_collada=False, filter_alembic=False,
                          filter_usd=False, filter_volume=False, filter_folder=True, filter_blenlib=False, filemode=9, show_multiview=False,
                          use_multiview=False, display_type='DEFAULT', sort_method='FILE_SORT_ALPHA', full=True)

bpy.ops.wm.quit_blender()
