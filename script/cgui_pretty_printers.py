import gdb

def make_gdb_string(string):
    return gdb.Value(string).cast(gdb.lookup_type('char').pointer())

class CguiNodePrinter:
    def __init__(self, val):
        self.isnull = not bool(val)
        if val.type.code == gdb.TYPE_CODE_PTR:
            try:
                self.node = val.dereference()
            except Exception:
                self.node = None
        else:
            self.node = val

    def to_string(self):
        if not self.node:
            if self.isnull:
                return 'CguiNode: <null>'
            else:
                return 'CguiNode: <invalid>'
        try:
            name = self.node['name']
            if name:
                return f'CguiNode: {name.string()}'
            else:
                return f'CguiNode: <null>'
        except Exception:
            return 'CguiNode: <invalid>'

    def children(self):
        if not self.node:
            return

        # Early display few fields
        early_fields = ['name', 'transformation', 'childrenCount', 'childrenCapacity', 'instancesCount', 'instancesCapacity']
        for early_field in early_fields:
            try:
                yield (early_field, self.node[early_field])
            except Exception:
                pass

        # Display children
        try:
            count = int(self.node['childrenCount'])
            ptr = self.node['children']
            if ptr:
                for idx in range(count):
                    try:
                        child = (ptr + idx).dereference()
                        if child:
                            yield (f'children[{idx}]', child)
                        else:
                            yield (f'children[{idx}]', make_gdb_string('<null>'))
                    except Exception:
                        yield (f'children[{idx}]', make_gdb_string('<invalid>'))
        except Exception:
            pass

        # Display instances
        try:
            count = int(self.node['instancesCount'])
            ptr = self.node['instances']
            if ptr:
                for idx in range(count):
                    try:
                        child = (ptr + idx).dereference()
                        if child:
                            yield (f'instances[{idx}]', child)
                        else:
                            yield (f'instances[{idx}]', make_gdb_string('<null>'))
                    except Exception:
                        yield (f'instances[{idx}]', make_gdb_string('<invalid>'))
        except Exception:
            pass

        # Display references
        ref_fields = ['parent', 'templateSource']
        for ref_field in ref_fields:
            try:
                ref_field_ptr = self.node[ref_field]
                if ref_field_ptr:
                    yield (ref_field, ref_field_ptr.dereference()['name'])
                else:
                    yield (ref_field, make_gdb_string('<null>'))
            except Exception:
                yield (ref_field, make_gdb_string('<invalid>'))

        # Display other fields
        handled_fields = early_fields + ref_fields
        try:
            for field in self.node.type.fields():
                field_name = field.name
                if field_name and field_name not in handled_fields:
                    try:
                        yield (field_name, self.node[field_name])
                    except Exception:
                        yield (field_name, make_gdb_string('<invalid>'))
        except Exception:
            pass

class CguiAnimatedTexturePrinter:
    def __init__(self, val):
        self.isnull = not bool(val)
        if val.type.code == gdb.TYPE_CODE_PTR:
            try:
                self.texture = val.dereference()
            except Exception:
                self.texture = None
        else:
            self.texture = val

    def to_string(self):
        if not self.texture:
            if self.isnull:
                return 'CguiAnimatedTexture: <null>'
            else:
                return 'CguiAnimatedTexture: <invalid>'
        try:
            count = int(self.texture['framesCount'])
            return f'CguiAnimatedTexture: {count} frame(s)'
        except Exception:
            return 'CguiAnimatedTexture: <invalid>'

    def children(self):
        if not self.texture:
            return

        # Early display few fields
        early_fields = ['framesCount', 'framesCapacity']
        for early_field in early_fields:
            try:
                yield (early_field, self.texture[early_field])
            except Exception:
                pass

        # Display frames
        try:
            count = int(self.texture['framesCount'])
            ptr = self.texture['frames']
            if ptr:
                for idx in range(count):
                    try:
                        frame = (ptr + idx).dereference()
                        if frame:
                            yield (f'frames[{idx}]', frame)
                        else:
                            yield (f'frames[{idx}]', make_gdb_string('<null>'))
                    except Exception:
                        yield (f'frames[{idx}]', make_gdb_string('<invalid>'))
        except Exception:
            pass

        # Display other fields
        handled_fields = early_fields + ['frames']
        try:
            for field in self.texture.type.fields():
                field_name = field.name
                if field_name and field_name not in handled_fields:
                    try:
                        yield (field_name, self.texture[field_name])
                    except Exception:
                        yield (field_name, make_gdb_string('<invalid>'))
        except Exception:
            pass

class CguiGridLayoutDataPrinter:
    def __init__(self, val):
        self.isnull = not bool(val)
        if val.type.code == gdb.TYPE_CODE_PTR:
            try:
                self.layout = val.dereference()
            except Exception:
                self.layout = None
        else:
            self.layout = val

    def to_string(self):
        if not self.layout:
            if self.isnull:
                return 'CguiGridLayoutData: <null>'
            else:
                return 'CguiGridLayoutData: <invalid>'
        try:
            x_count = int(self.layout['xSlotsCount'])
            y_count = int(self.layout['ySlotsCount'])
            return f'CguiGridLayoutData: {x_count} x-slots, {y_count} y-slots'
        except Exception:
            return 'CguiGridLayoutData: <invalid>'

    def children(self):
        if not self.layout:
            return

        # Early display few fields
        early_fields = ['xSlotsCount', 'xSlotsCapacity', 'ySlotsCount', 'ySlotsCapacity']
        for early_field in early_fields:
            try:
                yield (early_field, self.layout[early_field])
            except Exception:
                pass

        # Display xSlots
        try:
            x_count = int(self.layout['xSlotsCount'])
            x_ptr = self.layout['xSlots']
            if x_ptr:
                for idx in range(x_count):
                    try:
                        slot = (x_ptr + idx).dereference()
                        if slot:
                            yield (f'xSlots[{idx}]', slot)
                        else:
                            yield (f'xSlots[{idx}]', make_gdb_string('<null>'))
                    except Exception:
                        yield (f'xSlots[{idx}]', make_gdb_string('<invalid>'))
        except Exception:
            pass

        # Display ySlots
        try:
            y_count = int(self.layout['ySlotsCount'])
            y_ptr = self.layout['ySlots']
            if y_ptr:
                for idx in range(y_count):
                    try:
                        slot = (y_ptr + idx).dereference()
                        if slot:
                            yield (f'ySlots[{idx}]', slot)
                        else:
                            yield (f'ySlots[{idx}]', make_gdb_string('<null>'))
                    except Exception:
                        yield (f'ySlots[{idx}]', make_gdb_string('<invalid>'))
        except Exception:
            pass

        # Display other fields
        handled_fields = early_fields + ['xSlots', 'ySlots']
        try:
            for field in self.layout.type.fields():
                field_name = field.name
                if field_name and field_name not in handled_fields:
                    try:
                        yield (field_name, self.layout[field_name])
                    except Exception:
                        yield (field_name, make_gdb_string('<invalid>'))
        except Exception:
            pass

class CguiThemePrinter:
    def __init__(self, val):
        self.isnull = not bool(val)
        if val.type.code == gdb.TYPE_CODE_PTR:
            try:
                self.theme = val.dereference()
            except Exception:
                self.theme = None
        else:
            self.theme = val

    def to_string(self):
        if not self.theme:
            if self.isnull:
                return 'CguiTheme: <null>'
            else:
                return 'CguiTheme: <invalid>'
        try:
            name = self.theme['themeName']
            if name:
                return f'CguiTheme: {name.string()}'
            else:
                return 'CguiTheme: <null>'
        except Exception:
            return 'CguiTheme: <invalid>'

    def children(self):
        if not self.theme:
            return

        # Early display few fields
        early_fields = ['themeName']
        for early_field in early_fields:
            try:
                yield (early_field, self.theme[early_field])
            except Exception:
                pass

        # Display templates (fixed count from CGUI_COMPONENT_MAX)
        try:
            cgui_max = 0
            try:
                cgui_max = int(gdb.parse_and_eval('CGUI_COMPONENT_MAX'))
            except Exception:
                pass

            ptr = self.theme['templates']
            if ptr and cgui_max > 0:
                for idx in range(cgui_max):
                    try:
                        template_ptr = (ptr + idx).dereference()
                        if template_ptr:
                            yield (f'templates[{idx}]', template_ptr)
                        else:
                            yield (f'templates[{idx}]', make_gdb_string('<null>'))
                    except Exception:
                        yield (f'templates[{idx}]', make_gdb_string('<invalid>'))
        except Exception:
            pass

        # Display other fields
        handled_fields = early_fields + ['templates']
        try:
            for field in self.theme.type.fields():
                field_name = field.name
                if field_name and field_name not in handled_fields:
                    try:
                        yield (field_name, self.theme[field_name])
                    except Exception:
                        yield (field_name, make_gdb_string('<invalid>'))
        except Exception:
            pass

def lookup_cgui_printers(val):
    t = val.type.strip_typedefs()
    if t.code == gdb.TYPE_CODE_PTR:
        t = t.target().strip_typedefs()

    printer = None

    if t.tag and 'CguiNode' in t.tag:
        printer = CguiNodePrinter(val)
    elif t.tag and 'CguiAnimatedTexture' in t.tag:
        printer = CguiAnimatedTexturePrinter(val)
    elif t.tag and 'CguiGridLayoutData' in t.tag:
        printer = CguiGridLayoutDataPrinter(val)
    elif t.tag and 'CguiTheme' in t.tag:
        printer = CguiThemePrinter(val)

    if printer is not None:
        try:
            printer.to_string()
            printer.children()
            return printer
        except Exception:
            pass

    return None

gdb.pretty_printers.append(lookup_cgui_printers)
