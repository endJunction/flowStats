/**
 * \file
 * \author Dmitri Naumov (dmitri.naumov@ufz.de)
 *
 * \copyright 2012 -- Helmholtz Centre for Environmental Research - UFZ
 * distributed under the GNU GPL version 3. See the LICENSE file for details.
 *
 * \brief Difference to mean field.
 *
 * Calculates difference of fields U and p to their average values stored at
 * time zero. The average fields must be named UMean and pMean respectively.
 */

#include "fvCFD.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    timeSelector::addOptions();

    #include "setRootCase.H"
    #include "createTime.H"

    instantList timeDirs = timeSelector::select0(runTime, args);

    #include "createMesh.H"

    runTime.setTime(0, 0);

    #include "createFields.H"

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);

        Info<< "Time = " << runTime.timeName() << endl;

        IOobject pheader
        (
            "p",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        );

        if (!pheader.headerOk())
        {
            Info << "Unable to read p" << endl;
            return EXIT_FAILURE;
        }

        Info << "    Reading p" << endl;
        volScalarField p(pheader, mesh);

        IOobject Uheader
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        );

        if (!Uheader.headerOk())
        {
            Info << "Unable to read U" << endl;
            return EXIT_FAILURE;
        }

        Info << "    Reading U" << endl;
        volVectorField U(Uheader, mesh);

        pDiff = (p - pMean);
        UDiff = (U - UMean);

        pDiff.write();
        UDiff.write();

        Info<< endl;
    }

    return 0;
}


// ************************************************************************* //
