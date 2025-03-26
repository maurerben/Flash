module dataProcessing_F_interface

    use iso_c_binding

    implicit none

    interface
        subroutine electrons_write(filename, num_kpoints, num_bands, energies, occupations, length, overwrite) bind(C, name="electrons_write_double")
            import :: c_char, c_int, c_double, c_bool
            character(kind=c_char), dimension(*) :: filename
            integer(c_int), value :: num_kpoints, num_bands, length
            real(c_double), dimension(*) :: energies, occupations
            logical(c_bool), value :: overwrite
        end subroutine

!        subroutine electrons_read(filename, num_kpoints, num_bands, energies, occupations, length) bind(C, name="electrons_write_double")
!            use iso_c_binding
!            import :: c_char, c_int, c_double
!            character(kind=c_char), dimension(*) :: filename
!            integer(c_int) :: num_kpoints, num_bands
!            real(c_double), dimension(*) :: energies, occupations
!            integer(c_int), value :: length
!        end subroutine
    end interface
end module
